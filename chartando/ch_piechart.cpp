#include <QPieSeries>

#include "ch_piechart.h"

ch_PieChart::ch_PieChart(QString _title): ch_Chart(_title) {}

QChartView* ch_PieChart::draw(const QVector<ch_Sale*>& sales, QDate startDate, QDate endDate) const  {
    QMap<QString, double> chartMap;

    QPieSeries* pieSeries = new QPieSeries();
    QStringList pieLegend;

    for (QVector<ch_Sale*>::const_iterator it = sales.begin();
         it != sales.end(); ++it) {
        if (!startDate.isValid() || !endDate.isValid()
            || ((*it)->getSaleDate() >= startDate && (*it)->getSaleDate() <= endDate)) {
            QString sellerName = (*it)->getSellerName();

            if (!chartMap.contains(sellerName)) {
                chartMap.insert(sellerName, 0.0);
                pieLegend << sellerName;
            }

            chartMap[sellerName] = chartMap.value(sellerName) + (*it)->getSaleTotal();
        }
    }

    if (pieLegend.empty())
        return nullptr;

    QStringList::const_iterator legIt;
    for (legIt = pieLegend.begin(); legIt != pieLegend.end(); ++legIt) {
        QPieSlice* pieSlice = new QPieSlice(*legIt, chartMap.value(*legIt));
        pieSlice->setLabelVisible();

        pieSeries->append(pieSlice);
    }

    QChart* chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle(getTitle());
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}
