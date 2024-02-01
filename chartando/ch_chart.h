#ifndef CH_CHART_H
#define CH_CHART_H

#include <QString>
#include <QDate>
#include <QVector>
#include <QtCharts>

#include "ch_sale.h"

class ch_Chart {
    QString title;
public:
    ch_Chart(QString _title = "");
    virtual ~ch_Chart() = default;

    QString getTitle() const;

    virtual QChartView* draw(const QVector<ch_Sale*>& sales,
                             QDate startDate = QDate(), QDate endDate = QDate()) const = 0;
};

#endif // CH_CHART_H
