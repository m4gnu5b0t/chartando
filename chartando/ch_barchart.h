#ifndef CH_BARCHART_H
#define CH_BARCHART_H

#include "ch_chart.h"

class ch_BarChart: public ch_Chart {
public:
    ch_BarChart(QString _title = "Bar Chart");

    QChartView* draw(const QVector<ch_Sale*>& sales, QDate startDate, QDate endDate) const override;
};

#endif // CH_BARCHART_H
