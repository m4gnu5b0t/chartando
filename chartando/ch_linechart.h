#ifndef CH_LINECHART_H
#define CH_LINECHART_H

#include "ch_chart.h"

class ch_LineChart: public ch_Chart {
public:
    ch_LineChart(QString _title = "Line Chart");

    QChartView* draw(const QVector<ch_Sale*>& sales, QDate startDate, QDate endDate) const override;
};

#endif // CH_LINECHART_H
