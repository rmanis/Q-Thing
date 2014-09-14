
#include <QApplication>
#include "CrmGlWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    CrmGlWindow myWidget;
    myWidget.show();

    return app.exec();
}
