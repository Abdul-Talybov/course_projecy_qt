#ifndef FINDWINDOW_H
#define FINDWINDOW_H

#include "mainwindow.h"
#include <QMainWindow>
#include "Knot.h"
#include <vector>

namespace Ui {
class findwindow;
}

class findwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit findwindow(QWidget *parent = nullptr);
    ~findwindow();

    void AddRow(const QString& date, const QString& bucket_index = "", const QString& direc_code = "", const QString& dispil_name = "", const QString& teacher_sur = "");
    std::vector<Key*> SearchByDateRange(Knot* root, Date start_date, Date end_date, const std::string& disciplin_name);

public slots:
    void findhash();
    void findSignal(Knot *root);

private:
    Ui::findwindow *ui;
    Knot* m_root;
};

#endif // FINDWINDOW_H
