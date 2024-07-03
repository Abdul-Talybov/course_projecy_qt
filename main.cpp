#include "mainwindow.h"
#include "startwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StartWindow startwin;
    startwin.show();
    return a.exec();
}
