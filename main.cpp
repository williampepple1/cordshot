#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Set application info
    QApplication::setApplicationName("Cordshot");
    QApplication::setOrganizationName("Cordshot");
    QApplication::setApplicationVersion("1.1.0");
    
    // Enable high DPI scaling
    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
