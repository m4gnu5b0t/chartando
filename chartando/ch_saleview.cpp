#include <QLabel>
#include <QLineEdit>
#include <QDialog>

#include "ch_saleview.h"
#include "ch_saleparser.h"
#include "ch_barchart.h"
#include "ch_piechart.h"
#include "ch_linechart.h"

ch_SaleView::ch_SaleView():
    chart(nullptr)
    , open(new QPushButton)
    , save(new QPushButton)
    , newSale(new QPushButton)
    , newPromo(new QPushButton)
    , closeDataset(new QPushButton)
    , barChart(new QPushButton)
    , pieChart(new QPushButton)
    , lineChart(new QPushButton)
    , setRange(new QCheckBox("Filtra per periodo"))
    , startDate(new QDateEdit(QDate::currentDate()))
    , endDate(new QDateEdit(QDate::currentDate()))
    , sales(QVector<ch_Sale*>()) {
    connect(open, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(save, SIGNAL(clicked()), this, SLOT(saveFile()));
    connect(newSale, SIGNAL(clicked()), this, SLOT(addSale()));
    connect(newPromo, SIGNAL(clicked()), this, SLOT(addPromo()));
    connect(closeDataset, SIGNAL(clicked()), this, SLOT(deleteDataset()));

    connect(setRange, SIGNAL(toggled(bool)), this, SLOT(activateRange(bool)));
    connect(startDate, SIGNAL(dateChanged(QDate)), this, SLOT(updateStartDate(QDate)));
    connect(endDate, SIGNAL(dateChanged(QDate)), this, SLOT(updateEndDate(QDate)));

    connect(barChart, &QPushButton::clicked,
            this, [=](){ drawChart(new ch_BarChart); });

    connect(pieChart, &QPushButton::clicked,
            this, [=](){ drawChart(new ch_PieChart); });

    connect(lineChart, &QPushButton::clicked,
            this, [=](){ drawChart(new ch_LineChart); });

    connect(this, SIGNAL(salesChanged()), this, SLOT(viewSales()));
    connect(this, &ch_SaleView::salesChanged,
            this, [=](){ drawChart(chart); });

    startDate->setDisplayFormat("dd/MM/yyyy");
    endDate->setDisplayFormat("dd/MM/yyyy");

    setRetainSize(startDate);
    setRetainSize(endDate);

    startDate->setVisible(false);
    endDate->setVisible(false);

    uintValidator = new QRegularExpressionValidator(QRegularExpression("^$|[0-9]{0,2}"), this);
    percentageValidator = new QRegularExpressionValidator(QRegularExpression("^$|[0-9]{0,2}|^(100)"), this);
    doubleValidator = new QRegularExpressionValidator(QRegularExpression("^$|[0-9]{0,2}\\.[0-9]{0,2}"), this);

    setIcon(open, "/ch_icons/open.png");
    setIcon(save, "/ch_icons/save.png");
    setIcon(newSale, "/ch_icons/sale.png");
    setIcon(newPromo, "/ch_icons/promo.png");
    setIcon(closeDataset, "/ch_icons/close.png");
    setIcon(barChart, "/ch_icons/bar.png");
    setIcon(pieChart, "/ch_icons/pie.png");
    setIcon(lineChart, "/ch_icons/line.png");

    fileMenu = new QHBoxLayout;
    fileMenu->addWidget(open);
    fileMenu->addWidget(save);

    datasetMenu = new QHBoxLayout;
    datasetMenu->addWidget(newSale);
    datasetMenu->addWidget(newPromo);
    datasetMenu->addWidget(closeDataset);

    chartMenu = new QHBoxLayout;
    chartMenu->addWidget(barChart);
    chartMenu->addWidget(pieChart);
    chartMenu->addWidget(lineChart);

    rangeMenu = new QHBoxLayout;
    rangeMenu->addWidget(setRange);
    rangeMenu->addWidget(startDate);
    rangeMenu->addWidget(endDate);

    datasetScrollArea = new QScrollArea;
    datasetScrollArea->setBackgroundRole(QPalette::Dark);
    datasetScrollArea->setWidgetResizable(true);

    datasetWidget = new QWidget;
    datasetScrollArea->setWidget(datasetWidget);

    dataset_layout = new QGridLayout(datasetWidget);
    dataset_layout->setSizeConstraint(QLayout::SetMinimumSize);

    chart_layout = new QVBoxLayout;

    main_layout = new QGridLayout(this);
    main_layout->addLayout(fileMenu, 0, 0, 1, 2, Qt::AlignLeft);
    main_layout->addLayout(datasetMenu, 0, 2, 1, 3, Qt::AlignLeft);
    main_layout->addLayout(chartMenu, 0, 9, 1, 3, Qt::AlignLeft);
    main_layout->addLayout(rangeMenu, 0, 12, 1, 3, Qt::AlignLeft);
    main_layout->addWidget(datasetScrollArea, 2, 0, 100, 8);
    main_layout->addLayout(chart_layout, 1, 9, 100, 6, Qt::AlignJustify);

    showMaximized();
}

ch_SaleView::~ch_SaleView() {
    deleteSales();

    delete uintValidator;
    delete percentageValidator;
    delete doubleValidator;

    delete chart;
    deleteAllWidgets(main_layout);
}

void ch_SaleView::showMessage(const QString& message) {
    QDialog* dialog = new QDialog();
    QGridLayout* dialog_layout = new QGridLayout(dialog);

    QLabel* label = new QLabel(message);
    QPushButton* confirm = new QPushButton("OK");

    connect(confirm, SIGNAL(clicked()), dialog, SLOT(accept()));

    dialog_layout->addWidget(label, 0, 0);
    dialog_layout->addWidget(confirm, 1, 4);

    dialog->setModal(true);
    dialog->exec();

    deleteAllWidgets(dialog_layout);
    delete dialog;
}

void ch_SaleView::setIcon(QPushButton* pushButton, const QString& path) {
    QPixmap pixmap(QCoreApplication::applicationDirPath() + path);
    QIcon ButtonIcon(pixmap);

    pushButton->setIcon(ButtonIcon);
    pushButton->setIconSize(pixmap.rect().size());
}

void ch_SaleView::setRetainSize(QWidget* widget) {
    QSizePolicy sp_retain = widget->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);

    widget->setSizePolicy(sp_retain);
}

void ch_SaleView::setSaleHeaders(QGridLayout* _layout, unsigned int row) {
    _layout->addWidget(new QLabel("Tipo"), row, 0, Qt::AlignTop);
    _layout->addWidget(new QLabel("Venditore"), row, 1, Qt::AlignTop);
    _layout->addWidget(new QLabel("Prodotto"), row, 2, Qt::AlignTop);
    _layout->addWidget(new QLabel("Data vendita"), row, 3, Qt::AlignTop);
    _layout->addWidget(new QLabel("€/mese"), row, 4, Qt::AlignTop);
    _layout->addWidget(new QLabel("Mesi"), row, 5, Qt::AlignTop);
}

void ch_SaleView::setPromoHeaders(QGridLayout* _layout, unsigned int row){
    setSaleHeaders(_layout, row);
    _layout->addWidget(new QLabel("Mesi promo"), row, 6, Qt::AlignTop);
    _layout->addWidget(new QLabel("% sconto"), row, 7, Qt::AlignTop);
}

void ch_SaleView::deleteAllWidgets(QLayout* layout) {
    if (layout != nullptr) {
        QLayoutItem* childItem = layout->takeAt(0);
        while (childItem != nullptr) {
            if (childItem->widget())
               delete childItem->widget();
            else if (childItem->layout() != nullptr)
                deleteAllWidgets(childItem->layout());

            delete childItem;
            childItem = layout->takeAt(0);
        }
    }
}

void ch_SaleView::openFile() {
    QDialog* dialog = new QDialog();
    QGridLayout* dialog_layout = new QGridLayout(dialog);

    QLabel* label = new QLabel("Vuoi aprire un nuovo file? "
                               "\nEventuali dati non salvati andranno persi.");

    QPushButton *confirm = new QPushButton("Conferma"),
                *cancel = new QPushButton("Annulla");

    connect(confirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));

    dialog_layout->addWidget(label, 0, 0);
    dialog_layout->addWidget(cancel, 1, 4);
    dialog_layout->addWidget(confirm, 1, 5);

    bool sChanged = false;

    dialog->setModal(true);
    if(dialog->exec() == QDialog::Accepted) {
        QString fileName = QFileDialog::getOpenFileName(this, "Seleziona un file .xml",
                                                        QCoreApplication::applicationDirPath(), "*.xml");

        if (!fileName.isEmpty()) {
            deleteSales();
            sChanged = true;

            try {
                sales = ch_SaleParser::readFromFile(fileName);
            } catch (QString exc) {
                showMessage("Il file selezionato non contiene dati importabili in chartando."
                            "\nErrori riscontrati:"
                            "\n*" + exc);
            }
        }
    }

    deleteAllWidgets(dialog_layout);
    delete dialog;

    if (sChanged)
        emit salesChanged();
}

void ch_SaleView::saveFile() {
    if (sales.empty())
        showMessage("Il dataset è vuoto."
                    "\nNon sono presenti vendite da salvare su file.");
    else {
        QString fileName = QFileDialog::getSaveFileName(this, "Seleziona un file .xml",
                                                        QCoreApplication::applicationDirPath(), "*.xml");

        if (!fileName.isEmpty()) {
            try {
                ch_SaleParser::saveToFile(fileName.endsWith(".xml") ? fileName : fileName.append(".xml")
                                          , sales);
                showMessage("Dataset salvato correttamente.");
            } catch (QString exc) {
                showMessage("Non è stato possibile salvare il dataset sul file selezionato."
                            "\nErrori riscontrati:"
                            "\n*" + exc);
            }
        }
    }
}

void ch_SaleView::activateRange(bool checked) {
    startDate->setVisible(checked);
    endDate->setVisible(checked);

    drawChart(chart);
}

void ch_SaleView::updateStartDate(const QDate& date) {
    if (endDate->date().isValid() && date > endDate->date()) {
        startDate->setDate(endDate->date());
        showMessage("La data di inizio non può essere successiva alla data di fine."
                    "\nLa data di inizio massima consentita in base all'attuale data di fine è "
                    + endDate->date().toString("dd/MM/yyyy") + ".");
    }

    drawChart(chart);
}

void ch_SaleView::updateEndDate(const QDate& date) {
    if (startDate->date().isValid() && date < startDate->date()) {
        endDate->setDate(startDate->date());
        showMessage("La data di fine non può essere precedente alla data di inizio."
                    "\nLa data di fine minima consentita in base all'attuale data di inizio è "
                    + startDate->date().toString("dd/MM/yyyy") + ".");
    }

    drawChart(chart);
}

void ch_SaleView::viewSales() {
    deleteAllWidgets(dataset_layout);

    if (!sales.empty()) {
        setPromoHeaders(dataset_layout, 0);

        unsigned int row = 1;
        QVector<ch_Sale*>::const_iterator it;
        for (it = sales.begin(); it != sales.end(); ++it, ++row) {
            QLabel* type = new QLabel;
            type->setPixmap(QPixmap::fromImage(QImage("ch_icons/sale.png")));
            QLineEdit *sellerName = new QLineEdit((*it)->getSellerName()),
                      *productName = new QLineEdit((*it)->getProductName()),
                      *monthlyPrice = new QLineEdit(QString::number((*it)->getMonthlyPrice(), 'f', 2)),
                      *months = new QLineEdit(QString::number((*it)->getMonths()));

            QDateEdit* saleDate = new QDateEdit((*it)->getSaleDate());

            monthlyPrice->setValidator(doubleValidator);
            months->setValidator(uintValidator);

            saleDate->setDisplayFormat("dd/MM/yyyy");

            connect(sellerName, &QLineEdit::editingFinished,
                    this, [=](){ editSellerName(sellerName, *it); });
            connect(productName, &QLineEdit::editingFinished,
                    this, [=](){ editProductName(productName, *it); });
            connect(saleDate, &QDateEdit::editingFinished,
                    this, [=](){ editSaleDate((QDate::fromString(saleDate->text(), "dd/MM/yyyy")), *it); });
            connect(monthlyPrice, &QLineEdit::editingFinished,
                    this, [=](){ editMonthlyPrice(monthlyPrice, *it); });
            connect(months, &QLineEdit::editingFinished,
                    this, [=](){ editMonths(months, *it); });

            dataset_layout->addWidget(type, row, 0, Qt::AlignTop);
            dataset_layout->addWidget(sellerName, row, 1, Qt::AlignTop);
            dataset_layout->addWidget(productName, row, 2, Qt::AlignTop);
            dataset_layout->addWidget(saleDate, row, 3, Qt::AlignTop);
            dataset_layout->addWidget(monthlyPrice, row, 4, Qt::AlignTop);
            dataset_layout->addWidget(months, row, 5, Qt::AlignTop);

            if (dynamic_cast<ch_PromoSale*>(*it)) {
                type->setPixmap(QPixmap::fromImage(QImage("ch_icons/promo.png")));

                QLineEdit *promoMonths = new QLineEdit(QString::number(static_cast<ch_PromoSale*>(*it)->getPromoMonths())),
                          *discountPercentage = new QLineEdit(QString::number(
                                                              static_cast<ch_PromoSale*>(*it)->getDiscountPercentage()));

                promoMonths->setValidator(uintValidator);
                discountPercentage->setValidator(percentageValidator);

                connect(promoMonths, &QLineEdit::editingFinished,
                        this, [=](){ editPromoMonths(promoMonths, static_cast<ch_PromoSale*>(*it)); });
                connect(discountPercentage, &QLineEdit::editingFinished,
                        this, [=](){ editDiscountPercentage(discountPercentage, static_cast<ch_PromoSale*>(*it)); });

                dataset_layout->addWidget(promoMonths, row, 6, Qt::AlignTop);
                dataset_layout->addWidget(discountPercentage, row, 7, Qt::AlignTop);
            }

            QPushButton* deleteRow = new QPushButton();
            setIcon(deleteRow, "/ch_icons/delete.png");

            connect(deleteRow, &QPushButton::clicked,
                    this, [=](){ deleteSale(*it); });

            dataset_layout->addWidget(deleteRow, row, 8, Qt::AlignTop);
        }

        QWidget* emptySpacer = new QWidget;
        emptySpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        dataset_layout->addWidget(emptySpacer, row, 0);
    }
}

void ch_SaleView::drawChart(ch_Chart* _chart) {
    if (chart != nullptr && chart != _chart)
        delete chart;

    deleteAllWidgets(chart_layout);
    chart = _chart;

    if (_chart != nullptr) {
        QDate _startDate = setRange->isChecked() ?
                           QDate::fromString(startDate->text(), "dd/MM/yyyy") : QDate(),
              _endDate = setRange->isChecked() ?
                         QDate::fromString(endDate->text(), "dd/MM/yyyy") : QDate();

        chart_layout->addWidget(chart->draw(sales, _startDate, _endDate));

        if (chart_layout->itemAt(0) == nullptr) {
            showMessage("Dati insufficienti per costruire il grafico selezionato."
                        "\nDopo aver applicato le opportune modifiche, seleziona nuovamente il grafico da visualizzare."
                        "\n\nRequisiti minimi (nel periodo impostato):"
                        "\n* bar, pie => almeno una vendita"
                        "\n* line => almeno due vendite in mesi distinti");

            delete chart;
            chart = nullptr;
        }
    }
}

void ch_SaleView::addSale() {
    QDialog* dialog = new QDialog();
    QGridLayout* sale_layout = new QGridLayout(dialog);

    setSaleHeaders(sale_layout, 0);

    QLabel* type = new QLabel;
    type->setPixmap(QPixmap::fromImage(QImage("ch_icons/sale.png")));

    QLineEdit *sellerName = new QLineEdit(),
              *productName = new QLineEdit(),
              *monthlyPrice = new QLineEdit(),
              *months = new QLineEdit();

    QDateEdit *saleDate = new QDateEdit(QDate::currentDate());

    monthlyPrice->setValidator(doubleValidator);
    months->setValidator(uintValidator);

    saleDate->setDisplayFormat("dd/MM/yyyy");

    sale_layout->addWidget(type, 1, 0);
    sale_layout->addWidget(sellerName, 1, 1);
    sale_layout->addWidget(productName, 1, 2);
    sale_layout->addWidget(saleDate, 1, 3);
    sale_layout->addWidget(monthlyPrice, 1, 4);
    sale_layout->addWidget(months, 1, 5);

    QPushButton *confirm = new QPushButton("Aggiungi"),
                *cancel = new QPushButton("Annulla");

    connect(confirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));

    sale_layout->addWidget(cancel, 2, 4);
    sale_layout->addWidget(confirm, 2, 5);

    dialog->setModal(true);

    bool saleAdded = false;
    while(!saleAdded && dialog->exec() == QDialog::Accepted) {
        if (!sellerName->text().isEmpty()
            && !productName->text().isEmpty()
            && !monthlyPrice->text().isEmpty()
            && !months->text().isEmpty()
            && monthlyPrice->text().toDouble() >= 0.0
            && months->text().toInt() >= 1) {
            sales.push_back(new ch_Sale(sellerName->text(),
                                        productName->text(),
                                        QDate::fromString(saleDate->text(), "dd/MM/yyyy"),
                                        monthlyPrice->text().toDouble(),
                                        months->text().toInt()));

            saleAdded = true;
        } else
            showMessage("La vendita non può essere inserita."
                        "\nVerifica di aver compilato correttamente tutti i campi.");
    }

    deleteAllWidgets(sale_layout);
    delete dialog;

    if (saleAdded)
        emit salesChanged();
}

void ch_SaleView::addPromo() {
    QDialog* dialog = new QDialog();
    QGridLayout* sale_layout = new QGridLayout(dialog);

    setPromoHeaders(sale_layout, 0);

    QLabel* type = new QLabel;
    type->setPixmap(QPixmap::fromImage(QImage("ch_icons/promo.png")));

    QLineEdit *sellerName = new QLineEdit(),
              *productName = new QLineEdit(),
              *monthlyPrice = new QLineEdit(),
              *months = new QLineEdit(),
              *promoMonths = new QLineEdit(),
              *discountPercentage = new QLineEdit();

    QDateEdit *saleDate = new QDateEdit(QDate::currentDate());

    monthlyPrice->setValidator(doubleValidator);
    months->setValidator(uintValidator);
    promoMonths->setValidator(uintValidator);
    discountPercentage->setValidator(percentageValidator);

    saleDate->setDisplayFormat("dd/MM/yyyy");

    sale_layout->addWidget(type, 1, 0);
    sale_layout->addWidget(sellerName, 1, 1);
    sale_layout->addWidget(productName, 1, 2);
    sale_layout->addWidget(saleDate, 1, 3);
    sale_layout->addWidget(monthlyPrice, 1, 4);
    sale_layout->addWidget(months, 1, 5);
    sale_layout->addWidget(promoMonths, 1, 6);
    sale_layout->addWidget(discountPercentage, 1, 7);

    QPushButton *confirm = new QPushButton("Aggiungi"),
                *cancel = new QPushButton("Annulla");

    connect(confirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));

    sale_layout->addWidget(cancel, 2, 6);
    sale_layout->addWidget(confirm, 2, 7);

    dialog->setModal(true);

    bool saleAdded = false;
    while(!saleAdded && dialog->exec() == QDialog::Accepted) {
        if (!sellerName->text().isEmpty()
            && !productName->text().isEmpty()
            && !monthlyPrice->text().isEmpty()
            && !months->text().isEmpty()
            && !promoMonths->text().isEmpty()
            && !discountPercentage->text().isEmpty()
            && monthlyPrice->text().toDouble() >= 0.0
            && months->text().toInt() >= 1
            && promoMonths->text().toInt() >= 1
            && promoMonths->text().toInt() <= months->text().toInt()
            && discountPercentage->text().toInt() >= 0
            && discountPercentage->text().toInt() <= 100) {
            sales.push_back(new ch_PromoSale(sellerName->text(),
                                             productName->text(),
                                             QDate::fromString(saleDate->text(), "dd/MM/yyyy"),
                                             monthlyPrice->text().toDouble(),
                                             months->text().toInt(),
                                             promoMonths->text().toInt(),
                                             discountPercentage->text().toInt()));

            saleAdded = true;
        } else
            showMessage("La vendita non può essere inserita."
                        "\nVerifica di aver compilato correttamente tutti i campi.");
    }

    deleteAllWidgets(sale_layout);
    delete dialog;

    if (saleAdded)
        emit salesChanged();
}

void ch_SaleView::deleteSale(ch_Sale* sale) {
    if (sale != nullptr) {
        delete sale;
        sales.removeOne(sale);

        emit salesChanged();
    }
}

void ch_SaleView::deleteSales() {
    foreach (const ch_Sale* _ptr, sales)
        delete _ptr;

    sales.clear();
}

void ch_SaleView::deleteDataset() {
    QDialog* dialog = new QDialog();
    QGridLayout* dialog_layout = new QGridLayout(dialog);

    QLabel* label = new QLabel("Vuoi cancellare tutte le vendite? "
                               "\nEventuali dati non salvati andranno persi.");

    QPushButton *confirm = new QPushButton("Conferma"),
                *cancel = new QPushButton("Annulla");

    connect(confirm, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));

    dialog_layout->addWidget(label, 0, 0);
    dialog_layout->addWidget(cancel, 1, 4);
    dialog_layout->addWidget(confirm, 1, 5);

    bool sChanged = false;

    dialog->setModal(true);
    if(dialog->exec() == QDialog::Accepted) {
            deleteSales();
            sChanged = true;
    }

    deleteAllWidgets(dialog_layout);
    delete dialog;

    if (sChanged)
        emit salesChanged();
}

void ch_SaleView::editSellerName(QLineEdit* sellerName, ch_Sale* sale) {
    if (sellerName->text().isEmpty()) {
        showMessage("La modifica non verrà applicata."
                    "\nVerifica che:"
                    "\n* il venditore non sia vuoto");
        sellerName->setText(sale->getSellerName());
    }
    else {
        sale->updateSellerName(sellerName->text());
        drawChart(chart);
    }
}

void ch_SaleView::editProductName(QLineEdit* productName, ch_Sale* sale) {
    if (productName->text().isEmpty()) {
        showMessage("La modifica non verrà applicata."
                    "\nVerifica che:"
                    "\n* il prodotto non sia vuoto");
        productName->setText(sale->getProductName());
    }
    else {
        sale->updateProductName(productName->text());
        drawChart(chart);
    }
}

void ch_SaleView::editSaleDate(QDate _saleDate, ch_Sale* sale) {
    sale->updateSaleDate(_saleDate);
    drawChart(chart);
}

void ch_SaleView::editMonthlyPrice(QLineEdit* monthlyPrice, ch_Sale* sale) {
    if (monthlyPrice->text().isEmpty()
        || monthlyPrice->text().toDouble() < 0.0) {
        showMessage("La modifica non verrà applicata."
                    "\nVerifica che:"
                    "\n* il prezzo mensile non sia vuoto"
                    "\n* il prezzo mensile sia maggiore o uguale a 0.00");
        monthlyPrice->setText(QString::number(sale->getMonthlyPrice(), 'f', 2));
    } else {
        sale->updateMonthlyPrice(monthlyPrice->text().toDouble());
        drawChart(chart);
    }
}

void ch_SaleView::editMonths(QLineEdit* months, ch_Sale* sale) {
    if (months->text().isEmpty()
        || months->text().toInt() < 1) {
        showMessage("La modifica non verrà applicata."
                    "\nVerifica che:"
                    "\n* il numero di mesi non sia vuoto"
                    "\n* il numero di mesi sia maggiore o uguale a 1");
        months->setText(QString::number(sale->getMonths()));
    }
    else if (dynamic_cast<ch_PromoSale*>(sale)
        && static_cast<ch_PromoSale*>(sale)->getPromoMonths() > months->text().toInt())  {
        showMessage("La modifica non verrà applicata."
                    "\nVerifica che:"
                    "\n* il numero di mesi totali sia maggiore o uguale al numero di mesi promo");
        months->setText(QString::number(sale->getMonths()));
    } else {
        sale->updateMonths(months->text().toInt());
        drawChart(chart);
    }
}

void ch_SaleView::editPromoMonths(QLineEdit* promoMonths, ch_PromoSale* sale) {
    if (promoMonths->text().isEmpty()
        || promoMonths->text().toInt() < 1
        || promoMonths->text().toInt() > sale->getMonths()) {
        showMessage("La modifica non verrà applicata."
                    "Verifica che:"
                    "\n* il numero di mesi promo non sia vuoto"
                    "\n* il numero di mesi promo sia almeno 1"
                    "\n* il numero di mesi promo sia minore o uguale al numero di mesi totali");
        promoMonths->setText(QString::number(sale->getPromoMonths()));
    } else {
        sale->updatePromoMonths(promoMonths->text().toInt());
        drawChart(chart);
    }
}

void ch_SaleView::editDiscountPercentage(QLineEdit* discountPercentage, ch_PromoSale* sale) {
    if (discountPercentage->text().isEmpty()
        || discountPercentage->text().toInt() < 0.0
        || discountPercentage->text().toInt() > 100.0) {
        showMessage("La modifica non verrà applicata."
                    "\nVerifica che:"
                    "\n* la % di sconto non sia vuota"
                    "\n* la % di sconto sia compresa tra 0 e 100");
        discountPercentage->setText(QString::number(sale->getDiscountPercentage(), 'f', 2));
    } else {
        sale->updateDiscountPercentage(discountPercentage->text().toInt());
        drawChart(chart);
    }
}
