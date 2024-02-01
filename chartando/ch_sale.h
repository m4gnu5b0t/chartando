#ifndef CH_SALE_H
#define CH_SALE_H

#include <QString>
#include <QDate>

class ch_Sale {
private:
    QString sellerName,
            productName;
    QDate saleDate;
    double monthlyPrice;
    unsigned int months;

public:
    ch_Sale(QString _sellerName = "", QString _productName = "",
            QDate _saleDate = QDate::currentDate(), double _monthlyPrice = 0.0, unsigned int _months = 1);
    virtual ~ch_Sale() = default;

    QString getSellerName() const;
    QString getProductName() const;
    QDate getSaleDate() const;
    double getMonthlyPrice() const;
    unsigned int getMonths() const;

    virtual double getSaleTotal() const;

    void updateSellerName(const QString&);
    void updateProductName(const QString&);
    void updateSaleDate(const QDate&);
    void updateMonthlyPrice(double);
    void updateMonths(unsigned int);
};

#endif // CH_SALE_H
