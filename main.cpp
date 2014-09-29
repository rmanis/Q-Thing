
#include <QApplication>
#include "CrmGlWindow.h"
#include "globalicious.h"

const char *directoryName;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QString appPath = QCoreApplication::applicationDirPath();
    QByteArray bytes = appPath.toLocal8Bit();
    directoryName = bytes.data();

    QGLFormat qglFormat;
    qglFormat.setVersion(4,1);
    qglFormat.setProfile(QGLFormat::CoreProfile);
    qglFormat.setSampleBuffers(true);

    CrmGlWindow myWidget(qglFormat);
    myWidget.show();

    return app.exec();
}
