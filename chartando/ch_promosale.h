#ifndef CH_PROMOSALE_H
#define CH_PROMOSALE_H

#include "ch_sale.h"

class ch_PromoSale: public ch_Sale {
private:
    unsigned int promoMonths,
                 discountPercentage;

public:
    ch_PromoSale(QString _sellerName = "", QString _productName = "",
                 QDate _saleDate = QDate::currentDate(),
                 double _monthlyPrice = 0.0, unsigned int _months = 1,
                 unsigned int _promoMonths = 1, unsigned int _discountPercentage = 0);

    unsigned int getPromoMonths() const;
    unsigned int getDiscountPercentage() const;

    double getSaleTotal() const override;

    void updatePromoMonths(unsigned int);
    void updateDiscountPercentage(unsigned int);
};

#endif // CH_PROMOSALE_H
