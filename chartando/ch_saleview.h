#ifndef CH_SALEVIEW_H
#define CH_SALEVIEW_H

#include <QApplication>
#include <QVector>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDateEdit>
#include <QRegularExpressionValidator>
#include <QCheckBox>
#include <QPushButton>

#include "ch_sale.h"
#include "ch_promosale.h"
#include "ch_chart.h"

class ch_SaleView: public QWidget {
    Q_OBJECT
private:
    ch_Chart* chart;

    QPushButton *open,
                *save,
                *newSale,
                *newPromo,
                *closeDataset,
                *barChart,
                *pieChart,
                *lineChart;

    QCheckBox* setRange;
    QDateEdit *startDate,
              *endDate;

    QScrollArea *datasetScrollArea;
    QWidget *datasetWidget;

    QVBoxLayout *chart_layout;
    QHBoxLayout *fileMenu, *datasetMenu, *chartMenu, *rangeMenu;
    QGridLayout *dataset_layout,
                *main_layout;

    QVector<ch_Sale*> sales;

    QRegularExpressionValidator *uintValidator,
                                *percentageValidator,
                                *doubleValidator;

public:
    ch_SaleView();
    ~ch_SaleView();

    static void showMessage(const QString&);
    static void setIcon(QPushButton*, const QString&);
    static void setRetainSize(QWidget*);
    static void setSaleHeaders(QGridLayout* _layout, unsigned int row = 0);
    static void setPromoHeaders(QGridLayout* _layout, unsigned int row = 0);
    static void deleteAllWidgets(QLayout*);

    void deleteSales();

public slots:
    void openFile();
    void saveFile();

    void activateRange(bool);
    void updateStartDate(const QDate&);
    void updateEndDate(const QDate&);

    void viewSales();
    void drawChart(ch_Chart* _chart);

    void addSale();
    void addPromo();
    void deleteSale(ch_Sale*);
    void deleteDataset();

    void editSellerName(QLineEdit*, ch_Sale*);
    void editProductName(QLineEdit*, ch_Sale*);
    void editSaleDate(QDate, ch_Sale*);
    void editMonthlyPrice(QLineEdit*, ch_Sale*);
    void editMonths(QLineEdit*, ch_Sale*);
    void editPromoMonths(QLineEdit*, ch_PromoSale*);
    void editDiscountPercentage(QLineEdit*, ch_PromoSale*);

signals:
    void salesChanged();
};

#endif // CH_SALEVIEW_H
