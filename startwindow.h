#ifndef STARTWINDOW_H
#define STARTWINDOW_H
#include "mainwindow.h"
#include <QDialog>

namespace Ui
{
class StartWindow;
}

class StartWindow: public QDialog
{
    Q_OBJECT;

public slots:
    void class_of_quantity_hash_table();

signals:
    void ToMainSignal(int text, int filelength);

public:
    explicit StartWindow(QWidget *parent = nullptr);
    ~StartWindow();

private:
    Ui::StartWindow *ui;
    MainWindow* mainWin {};
};

#endif // STARTWINDOW_H
