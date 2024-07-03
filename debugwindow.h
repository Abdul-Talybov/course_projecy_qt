#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H
#include <mainwindow.h>
#include <QMainWindow>
#include <QTextEdit>
#include "Knot.h" // Включаем заголовочный файл для Knot, если он используется здесь

namespace Ui {
    class debugwindow;
}

class debugwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit debugwindow(QWidget *parent = nullptr);
    ~debugwindow();
    void Print_debug(Knot* t, int h, QTextEdit* textEdit, bool isRightChild = false);
    void printHashTablesInTree(Knot *root);
    void AddRow_D(const QString &date, const QString &bucket_index, const QString &direc_code, const QString &dispil_name, const QString &teacher_sur);
    void printHashTable(HashTable *hashTable, const std::string &date);
public slots:
    void debugSignal(Knot* root); // Публичный слот для получения указателя на корень дерева

private:
    Ui::debugwindow *ui; // Указатель на объект интерфейса
    Knot* d_root; // Указатель на корень дерева
};

#endif // DEBUGWINDOW_H
