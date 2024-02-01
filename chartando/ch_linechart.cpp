#include <QLineSeries>

#include "ch_linechart.h"

ch_LineChart::ch_LineChart(QString _title): ch_Chart(_title) {}

QChartView* ch_LineChart::draw(const QVector<ch_Sale*>& sales, QDate startDate, QDate endDate) const  {
    QMap<QDate, double> chartMap;

    QLineSeries* lineSeries = new QLineSeries();
    QList<QDate> lineLegend;

    QVector<ch_Sale*>::const_iterator it;
    for (it = sales.begin(); it != sales.end(); ++it) {
        QDate saleDate = QDate((*it)->getSaleDate().year(),
                               (*it)->getSaleDate().month() , 15);

        if (!startDate.isValid() || !endDate.isValid()
            || ((*it)->getSaleDate() >= startDate && (*it)->getSaleDate() <= endDate)) {
            if (!chartMap.contains(saleDate)) {
                chartMap.insert(saleDate, 0.0);
                lineLegend << saleDate;
            }

            chartMap[saleDate] = chartMap.value(saleDate) + (*it)->getSaleTotal();
        }
    }

    if (lineLegend.count() < 2)
        return nullptr;

    qSort(lineLegend.begin(), lineLegend.end());

    double maxRange = 0;

    QList<QDate>::const_iterator legIt;
    for (legIt = lineLegend.begin();
         legIt != lineLegend.end(); ++legIt) {
        lineSeries->append(QDateTime(*legIt).toMSecsSinceEpoch(), chartMap.value(*legIt));

        if (chartMap.value(*legIt) > maxRange)
            maxRange = chartMap.value(*legIt);
    }

    QChart* chart = new QChart();
    chart->addSeries(lineSeries);
    chart->setTitle(getTitle());
    chart->legend()->setVisible(false);

    QDateTimeAxis* axisX = new QDateTimeAxis;
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Mesi");

    QValueAxis* axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Totale");
    axisY->setRange(0, static_cast<int>(maxRange) + 10 - (static_cast<int>(maxRange) % 10));
    axisY->setTickCount(4);

    chart->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);

    chart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisY);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}
