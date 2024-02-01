#include "ch_promosale.h"

ch_PromoSale::ch_PromoSale(QString _sellerName, QString _productName,
                           QDate _saleDate, double _monthlyPrice, unsigned int _months,
                           unsigned int _promoMonths, unsigned int _discountPercentage):
                           ch_Sale(_sellerName, _productName, _saleDate, _monthlyPrice, _months),
                           promoMonths(_promoMonths >= 1 && _promoMonths <= getMonths() ? _promoMonths : 1),
                           discountPercentage(_discountPercentage <= 100 ? _discountPercentage : 0) {}

unsigned int ch_PromoSale::getPromoMonths() const {
    return promoMonths;
}

unsigned int ch_PromoSale::getDiscountPercentage() const {
    return discountPercentage;
}

double ch_PromoSale::getSaleTotal() const {
    return getMonthlyPrice() * (getMonths() - promoMonths)
           + ((getMonthlyPrice() * discountPercentage) / 100) * promoMonths;
}

void ch_PromoSale::updatePromoMonths(unsigned int _promoMonths) {
    if (_promoMonths >= 1 && _promoMonths <= getMonths())
        promoMonths = _promoMonths;
}

void ch_PromoSale::updateDiscountPercentage(unsigned int _discountPercentage) {
    if (_discountPercentage <= 100)
        discountPercentage = _discountPercentage;
}
