#include "ch_sale.h"

ch_Sale::ch_Sale(QString _sellerName, QString _productName,
                 QDate _saleDate, double _monthlyPrice, unsigned int _months):
                 sellerName(_sellerName), productName(_productName),
                 saleDate(_saleDate.isValid() ? _saleDate : QDate::currentDate()),
                 monthlyPrice(_monthlyPrice >= 0.0 ? _monthlyPrice : 0.0),
                 months(_months >= 1 ? _months : 1) {}

QString ch_Sale::getSellerName() const {
    return sellerName;
}

QString ch_Sale::getProductName() const {
    return productName;
}

QDate ch_Sale::getSaleDate() const {
    return saleDate;
}

double ch_Sale::getMonthlyPrice() const {
    return monthlyPrice;
}

unsigned int ch_Sale::getMonths() const {
    return months;
}

double ch_Sale::getSaleTotal() const {
    return monthlyPrice * months;
}

void ch_Sale::updateSellerName(const QString& _sellerName) {
    sellerName = _sellerName;
}

void ch_Sale::updateProductName(const QString& _productName) {
    productName = _productName;
}

void ch_Sale::updateSaleDate(const QDate& _saleDate) {
    if (_saleDate.isValid())
        saleDate = _saleDate;
}

void ch_Sale::updateMonthlyPrice(double _monthlyPrice) {
    if (_monthlyPrice >= 0.0)
        monthlyPrice = _monthlyPrice;
}

void ch_Sale::updateMonths(unsigned int _months) {
    if (_months >= 1)
        months = _months;
}
