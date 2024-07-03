#include "startwindow.h"
#include "ui_startwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>

void StartWindow::class_of_quantity_hash_table()
{
    int filesize=0;
    qDebug()<<"Go to MainWindow";
    QFile inputFile("C:/Course/course_project_end/DataBase.txt");
    if (inputFile.open(QIODevice::ReadOnly)){
        QTextStream in(&inputFile);
        while(!in.atEnd()){
            filesize++;
            in.readLine();
        }
        inputFile.close();
    }
    mainWin->show();
    ui->AVL_TREE_ENTRANCE->setEnabled(false);
    int countofhash = ui->quantity_hash_table->value();
    emit ToMainSignal(countofhash, filesize);
}
StartWindow::StartWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartWindow)

{
    ui->setupUi(this);
    ui->quantity_hash_table->setMinimum(1);
    ui->quantity_hash_table->setMaximum(99);
    ui->textEdit->setReadOnly(true); // Устанавливаем текстовое поле только для чтения
    mainWin = new MainWindow(this);
    connect(ui->AVL_TREE_ENTRANCE, SIGNAL(clicked()), this,SLOT(class_of_quantity_hash_table()));
    connect(this,&StartWindow::ToMainSignal,mainWin, &MainWindow::startSignal);
}

StartWindow::~StartWindow()
{
    delete ui;
}
