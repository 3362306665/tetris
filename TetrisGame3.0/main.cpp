#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;Game_interface w2;
    //Game_interface g;
    w.show();
    //g.show();
    return a.exec();

}
