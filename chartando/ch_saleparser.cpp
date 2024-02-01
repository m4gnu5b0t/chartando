#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include "ch_saleparser.h"
#include "ch_promosale.h"

QVector<ch_Sale*> ch_SaleParser::readFromFile(const QString& fileName) {
    QVector<ch_Sale*> aux;

    QDomDocument domXML;
    QFile xmlFile(fileName);
    if (!xmlFile.open(QIODevice::ReadOnly))
        throw QString("impossibile aprire il file selezionato");

    domXML.setContent(&xmlFile);
    xmlFile.close();

    QDomElement domSales = domXML.documentElement();
    if (domSales.tagName() != "ch_Sales")
        throw "il file selezionato non è valido";

    QDomElement domSale = domSales.firstChild().toElement();
    while(!domSale.isNull()) {
        if (domSale.tagName()=="ch_Sale") {
            ch_Sale* sale = new ch_Sale();

            QDomElement saleDetail = domSale.firstChild().toElement();
            while (!saleDetail.isNull())  {
                if (saleDetail.tagName()=="sellerName")
                    sale->updateSellerName(saleDetail.firstChild().toText().data());
                else if (saleDetail.tagName()=="productName")
                    sale->updateProductName(saleDetail.firstChild().toText().data());
                else if (saleDetail.tagName()=="saleDate")
                    sale->updateSaleDate(QDate::fromString(saleDetail.firstChild().toText().data(),
                                         "yyyy-MM-dd"));
                else if (saleDetail.tagName()=="monthlyPrice")
                    sale->updateMonthlyPrice(saleDetail.firstChild().toText().data().toDouble());
                else if (saleDetail.tagName()=="months")
                    sale->updateMonths(saleDetail.firstChild().toText().data().toInt());

                saleDetail = saleDetail.nextSibling().toElement();
            }

            aux.push_back(sale);
        } else if (domSale.tagName() == "ch_PromoSale") {
            ch_PromoSale* sale = new ch_PromoSale();

            QDomElement saleDetail = domSale.firstChild().toElement();
            while (!saleDetail.isNull())  {
                if (saleDetail.tagName()=="sellerName")
                    sale->updateSellerName(saleDetail.firstChild().toText().data());
                else if (saleDetail.tagName()=="productName")
                    sale->updateProductName(saleDetail.firstChild().toText().data());
                else if (saleDetail.tagName()=="saleDate")
                    sale->updateSaleDate(QDate::fromString(saleDetail.firstChild().toText().data(),
                                         "yyyy-MM-dd"));
                else if (saleDetail.tagName()=="monthlyPrice")
                    sale->updateMonthlyPrice(saleDetail.firstChild().toText().data().toDouble());
                else if (saleDetail.tagName()=="months")
                    sale->updateMonths(saleDetail.firstChild().toText().data().toInt());
                else if (saleDetail.tagName()=="promoMonths")
                    sale->updatePromoMonths(saleDetail.firstChild().toText().data().toInt());
                else if (saleDetail.tagName()=="discountPercentage")
                    sale->updateDiscountPercentage(saleDetail.firstChild().toText().data().toInt());

                saleDetail = saleDetail.nextSibling().toElement();
            }

            aux.push_back(sale);
        }

        domSale = domSale.nextSibling().toElement();
    }

    if(aux.empty())
        throw "il file selezionato è vuoto oppure non è valido";

    return aux;
}

void ch_SaleParser::saveToFile(const QString& fileName, const QVector<ch_Sale*>& sales) {
    QDomDocument domXML;
    QFile xmlFile(fileName);
    if (!xmlFile.open(QIODevice::WriteOnly))
        throw "impossibile aprire il file selezionato";

    QDomElement domSales = domXML.createElement("ch_Sales");
    domXML.appendChild(domSales);

    foreach (const ch_Sale* _ptr, sales) {
        QDomElement domSale, domSaleDetail;

        if (!dynamic_cast<const ch_PromoSale*>(_ptr)) {
            domSale = domXML.createElement("ch_Sale");
            domSales.appendChild(domSale);

            domSaleDetail = domXML.createElement("sellerName");
            domSaleDetail.appendChild(domXML.createTextNode((_ptr ->getSellerName())));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("productName");
            domSaleDetail.appendChild(domXML.createTextNode((_ptr ->getProductName())));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("saleDate");
            domSaleDetail.appendChild(domXML.createTextNode((_ptr ->getSaleDate().toString("yyyy-MM-dd"))));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("monthlyPrice");
            domSaleDetail.appendChild(domXML.createTextNode((QString::number(_ptr ->getMonthlyPrice(), 'f', 2))));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("months");
            domSaleDetail.appendChild(domXML.createTextNode((QString::number(_ptr ->getMonths()))));
            domSale.appendChild(domSaleDetail);
        } else {
            domSale = domXML.createElement("ch_PromoSale");
            domSales.appendChild(domSale);

            domSaleDetail = domXML.createElement("sellerName");
            domSaleDetail.appendChild(domXML.createTextNode((_ptr ->getSellerName())));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("productName");
            domSaleDetail.appendChild(domXML.createTextNode((_ptr ->getProductName())));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("saleDate");
            domSaleDetail.appendChild(domXML.createTextNode((_ptr ->getSaleDate().toString("yyyy-MM-dd"))));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("monthlyPrice");
            domSaleDetail.appendChild(domXML.createTextNode((QString::number(_ptr ->getMonthlyPrice(), 'f', 2))));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("months");
            domSaleDetail.appendChild(domXML.createTextNode((QString::number(_ptr ->getMonths()))));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("promoMonths");
            domSaleDetail.appendChild(domXML.createTextNode((QString::number(
                          static_cast<const ch_PromoSale*>(_ptr)->getPromoMonths()))));
            domSale.appendChild(domSaleDetail);

            domSaleDetail = domXML.createElement("discountPercentage");
            domSaleDetail.appendChild(domXML.createTextNode((QString::number(
                          static_cast<const ch_PromoSale*>(_ptr)->getDiscountPercentage()))));
            domSale.appendChild(domSaleDetail);
        }
    }

    QTextStream xmlStream(&xmlFile);
    xmlStream << domXML.toString();

    xmlFile.close();
}
