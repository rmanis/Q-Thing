
#include <QApplication>
#include "CrmGlWindow.h"
#include "globalicious.h"

const char *directoryName;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString appPath = QCoreApplication::applicationDirPath();
    QByteArray bytes = appPath.toLocal8Bit();
    directoryName = bytes.data();

    CrmGlWindow myWidget;
    myWidget.show();

    return app.exec();
}
