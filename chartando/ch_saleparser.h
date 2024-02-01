#ifndef CH_SALEPARSER_H
#define CH_SALEPARSER_H

#include <QVector>

#include "ch_sale.h"

class ch_SaleParser {
public:
    static QVector<ch_Sale*> readFromFile(const QString&);
    static void saveToFile(const QString&, const QVector<ch_Sale*>&);
};

#endif // CH_SALEPARSER_H
