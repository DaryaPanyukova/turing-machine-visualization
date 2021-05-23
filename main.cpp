#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Turing machine");

    w.show();
    return a.exec();
}
