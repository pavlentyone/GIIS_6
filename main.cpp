#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Text Editor");
    a.setApplicationVersion("0.1");

    a.setOrganizationName("ExampleSoft");
    a.setOrganizationDomain("example.com");
    a.setWindowIcon(QIcon(":new/icons/new.png"));
    MainWindow w;
    w.show();

    return a.exec();
}
