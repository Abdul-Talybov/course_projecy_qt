#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTextStream>
#include "Knot.h"
#include "iostream"
class findwindow;
class debugwindow;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void startSignal(int string, int filelength);
    void Initial();
    void SaveBtn();
    void AddBtnBtn();
    void DelBtnBtn();
    void FindBtn();
    void DebugBtn();
public:

    void AddRow(HashNode* p);
    void WalkTree(Knot* t);
    void PrinterHashTable(HashTable* hashTable);
    void WalkTree_To_File(Knot *p, QTextStream &writestream);
    void PrinterHashTable_To_File(HashTable *hashTable, QTextStream &writestream);
signals:
    void ToFindSignal(Knot* root);
    void ToDebugSignal(Knot* root);
private:
    Ui::MainWindow *ui;
    findwindow* findWin {};
    debugwindow* debugWin {};
    Knot* root = nullptr;
    bool heighChange = false;
};
#endif // MAINWINDOW_H
