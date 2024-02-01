#ifndef CH_PIECHART_H
#define CH_PIECHART_H

#include "ch_chart.h"

class ch_PieChart: public ch_Chart {
public:
    ch_PieChart(QString _title = "Pie Chart");

    QChartView* draw(const QVector<ch_Sale*>& sales, QDate startDate, QDate endDate) const override;
};

#endif // CH_PIECHART_H
