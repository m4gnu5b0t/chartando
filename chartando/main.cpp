#include <QApplication>

#include "ch_saleview.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ch_SaleView saleView;
    saleView.show();

    return app.exec();
}
