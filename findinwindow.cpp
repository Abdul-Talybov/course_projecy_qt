#include "findinwindow.h"
#include "ui_findinwindow.h"

findinwindow::findinwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::findinwindow)
{
    ui->setupUi(this);
}

findinwindow::~findinwindow()
{
    delete ui;
}
