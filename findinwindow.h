#ifndef FINDINWINDOW_H
#define FINDINWINDOW_H

#include <QMainWindow>

namespace Ui {
class findinwindow;
}

class findinwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit findinwindow(QWidget *parent = nullptr);
    ~findinwindow();

private:
    Ui::findinwindow *ui;
};

#endif // FINDINWINDOW_H
