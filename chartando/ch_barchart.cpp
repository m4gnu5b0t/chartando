#include <QBarSet>
#include <QBarSeries>

#include "ch_barchart.h"

ch_BarChart::ch_BarChart(QString _title): ch_Chart(_title) {}

QChartView* ch_BarChart::draw(const QVector<ch_Sale*>& sales, QDate startDate, QDate endDate) const  {
    QMap<QString, QMap<QString, double>> chartMap;

    QBarSeries* barSeries = new QBarSeries;
    QStringList barLegend, barCategories;

    QVector<ch_Sale*>::const_iterator it;
    for (it = sales.begin(); it != sales.end(); ++it) {
        if (!startDate.isValid() || !endDate.isValid()
            || ((*it)->getSaleDate() >= startDate && (*it)->getSaleDate() <= endDate)) {
            QString sellerName = (*it)->getSellerName(),
                    productName = (*it)->getProductName();

            if (!chartMap.contains(sellerName)) {
                chartMap.insert(sellerName, QMap<QString, double>());
                barLegend << sellerName;
             }

            if (!chartMap.value(sellerName).contains(productName))
                chartMap[sellerName].insert(productName, 0.0);

            chartMap[sellerName][productName] = chartMap.value(sellerName).value(productName)
                                                + (*it)->getSaleTotal();

            if (!barCategories.contains(productName))
                barCategories << productName;
        }
    }

    if (barLegend.empty())
        return nullptr;

    barCategories.sort(Qt::CaseInsensitive);
    barLegend.sort(Qt::CaseInsensitive);

    double maxRange = 0;

    QStringList::const_iterator legIt;
    for (legIt = barLegend.begin(); legIt != barLegend.end(); ++legIt) {
        QBarSet* barSet = new QBarSet(*legIt);

        QStringList::const_iterator catIt;
        for (catIt = barCategories.begin(); catIt != barCategories.end(); ++catIt) {
            *barSet << chartMap.value(*legIt).value(*catIt, 0);

            if (chartMap.value(*legIt).value(*catIt, 0) > maxRange)
                maxRange = chartMap.value(*legIt).value(*catIt, 0);;
        }

        barSeries->append(barSet);
    }

    QChart* chart = new QChart();
    chart->addSeries(barSeries);
    chart->setTitle(getTitle());
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis* axisX = new QBarCategoryAxis;
    axisX->append(barCategories);

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, static_cast<int>(maxRange) + 10 - (static_cast<int>(maxRange) % 10));
    axisY->setTickCount(4);

    chart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}
