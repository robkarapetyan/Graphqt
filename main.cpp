#include "grapheditor.h"
#include "mainwindow.h"
#include "mywidget.h"
#include "node.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

   // QObject::connect(&w,SIGNAL(Sending_signal()),&f,SLOT(Receiving_slot()));
    //QObject::connect(&w,SIGNAL(asert()),&f,SLOT(showsize()));
    // QObject::connect(&w,SIGNAL(add()),&i,SLOT(showsize()));

    w.show();

    return a.exec();
}
