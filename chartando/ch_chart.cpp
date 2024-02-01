#include "ch_chart.h"

ch_Chart::ch_Chart(QString _title): title(_title) {}

QString ch_Chart::getTitle() const {
    return title;
}
