#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QWidget>
#include "common.hh"
#include "mainwindow.hh"

int main(int argc,char *argv[])
{

    QApplication app(argc,argv);

    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    if (QCoreApplication::arguments().contains(QStringLiteral("--multisample")))
        fmt.setSamples(4);
    if (QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"))) {
        fmt.setVersion(3, 2);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    }
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow *mw = new MainWindow();
    mw->show();
    return app.exec();

}

