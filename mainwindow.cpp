#include <iostream>
#include <clocale>
#include <list>
#include <fstream>
#include <sstream>
#include <QApplication>
#include <QFile>
#include <QString>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
#include <QTableWidget>
#include <QStringDecoder>
#include <QDate>

#include "Knot.h"
#include "ADCRingL.h"
#include "auxiliary_functions.h"
#include "findwindow.h"
#include "debugwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Knot* q;
bool KeyBelowX(Knot* est, Key* key) {
    if (est->key->date.year > key->date.year) {
        return true;
    }
    if (est->key->date.year < key->date.year) {
        return false;
    }
    if (est->key->date.month > key->date.month) {
        return true;
    }
    if (est->key->date.month < key->date.month) {
        return false;
    }
    if (est->key->date.day > key->date.day) {
        return true;
    }
    return false;
}

bool KeyAboveX(Knot* est, Key* key) {
    if (est->key->date.year < key->date.year) {
        return true;
    }
    if (est->key->date.year > key->date.year) {
        return false;
    }
    if (est->key->date.month < key->date.month) {
        return true;
    }
    if (est->key->date.month > key->date.month) {
        return false;
    }
    if (est->key->date.day < key->date.day) {
        return true;
    }
    return false;
}


bool XEquallyKnot(Knot* est, Key* key) {
    if (
        (est->key->date.year == key->date.year) &&
        (est->key->date.month == key->date.month) &&
        (est->key->date.day == key->date.day)
        )
    {
        return true;
    }
    return false;
}

Date stringToDate(std::string s) {
    Date date;
    date.day = std::stoi(s.substr(0, 2));
    date.month = std::stoi(s.substr(3, 2));
    date.year = std::stoi(s.substr(6, 4));
    return date;
}

Direction_code stringToDirCode(std::string s) {
    if (s.empty()){
        throw std::invalid_argument("Input string is empty");
    }
    Direction_code dir_code;

    dir_code.BB = std::stoi(s.substr(0, 2));
    dir_code.CC = std::stoi(s.substr(3, 2));
    dir_code.DD = std::stoi(s.substr(6, 2));
    return dir_code;

}

void Search(Key* key, int y, Knot*& p, bool& h, int tableSize) {
    Knot* p1, * p2;
    if (p == nullptr) {
        p = new Knot(tableSize);
        p->key = key;
        Key* keyForHashTable = new Key();
        keyForHashTable = key;
        p->hashTable->insert(keyForHashTable);
        add(p->headp, y);
        p->bal = 0;
        h = 1;
        p->left = nullptr;
        p->right = nullptr;
    }
    else if (KeyAboveX(p, key)) {
        Search(key, y, p->left, h, tableSize);
        if (h) {
            if (p->bal == 1) {
                p->bal = 0;
                h = false;
            }
            else if (p->bal == 0) {
                p->bal = -1;
            }
            else {
                p1 = p->left;
                if (p1->bal == -1) {
                    p->left = p1->right;
                    p1->right = p;
                    p->bal = 0;
                    p = p1;
                }
                else {
                    p2 = p1->right;
                    p1->right = p2->left;
                    p2->left = p1;
                    p->left = p2->right;
                    p2->right = p;
                    if (p2->bal == -1) p->bal = 1;
                    else p->bal = 0;
                    if (p2->bal == 1) p1->bal = -1;
                    else p1->bal = 0;
                    p = p2;
                }
                p->bal = 0;
                h = 0;
            }
        }
    }
    else if (KeyBelowX(p, key)) {
        Search(key, y, p->right, h, tableSize);
        if (h) {
            if (p->bal == -1) {
                p->bal = 0;
                h = false;
            }
            else if (p->bal == 0)
                p->bal = 1;
            else {
                p1 = p->right;
                if (p1->bal == 1) {
                    p->right = p1->left;
                    p1->left = p;
                    p->bal = 0;
                    p = p1;
                }
                else {
                    p2 = p1->left;
                    p1->left = p2->right;
                    p2->right = p1;
                    p->right = p2->left;
                    p2->left = p;
                    if (p2->bal == 1) p->bal = -1;
                    else p->bal = 0;
                    if (p2->bal == -1) p1->bal = 1;
                    else  p1->bal = 0;
                    p = p2;
                }
                p->bal = 0;
                h = 0;
            }
        }
    }
    else if (XEquallyKnot(p, key)) {
        add(p->headp, y);
        // Проверяем, существует ли уже teacher_name в хеш-таблице
        Key* existingKey = p->hashTable->search_for_hash(key);
        if (existingKey == nullptr) { // Если teacher_name не найдено, добавляем его
            Key* keyForHashTable = new Key();
            keyForHashTable = key;
            p->hashTable->insert(keyForHashTable);
        }
        h = 0;
    }
}

void SearchPerOnce(Knot*& root, std::string discipline_name, std::string teacher_name,
    int BB, int CC, int DD, int day, int month, int year, int tableSize) {
    Key* key = new Key();
    key->direction_code.BB = BB;
    key->direction_code.CC = CC;
    key->direction_code.DD = DD;
    key->date.day = day;
    key->date.month = month;
    key->date.year = year;
    key->discipline_name = discipline_name;
    key->teacher_name = teacher_name;
    bool h = false;
    Search(key, 0, root, h, tableSize);
}
bool flag;
bool SearchBool(Key* key, int y, Knot*& p, bool& h, int tableSize) {
    Knot* p1, * p2;
    if (p == nullptr) {
        p = new Knot(tableSize);
        p->key = key;
        Key* keyForHashTable = new Key();
        keyForHashTable = key;
        p->hashTable->insert(keyForHashTable);
        p->bal = 0;
        h = 1;
        p->left = nullptr;
        p->right = nullptr;
        flag = true;
    }
    else if (KeyAboveX(p, key)) {
        SearchBool(key, y, p->left, h, tableSize);
        if (h) {
            if (p->bal == 1) {
                p->bal = 0;
                h = false;
            }
            else if (p->bal == 0) {
                p->bal = -1;
            }
            else {
                p1 = p->left;
                if (p1->bal == -1) {
                    p->left = p1->right;
                    p1->right = p;
                    p->bal = 0;
                    p = p1;
                }
                else {
                    p2 = p1->right;
                    p1->right = p2->left;
                    p2->left = p1;
                    p->left = p2->right;
                    p2->right = p;
                    if (p2->bal == -1) p->bal = 1;
                    else p->bal = 0;
                    if (p2->bal == 1) p1->bal = -1;
                    else p1->bal = 0;
                    p = p2;
                }
                p->bal = 0;
                h = 0;
            }
        }
    }
    else if (KeyBelowX(p, key)) {
        SearchBool(key, y, p->right, h, tableSize);
        if (h) {
            if (p->bal == -1) {
                p->bal = 0;
                h = false;
            }
            else if (p->bal == 0)
                p->bal = 1;
            else {
                p1 = p->right;
                if (p1->bal == 1) {
                    p->right = p1->left;
                    p1->left = p;
                    p->bal = 0;
                    p = p1;
                }
                else {
                    p2 = p1->left;
                    p1->left = p2->right;
                    p2->right = p1;
                    p->right = p2->left;
                    p2->left = p;
                    if (p2->bal == 1) p->bal = -1;
                    else p->bal = 0;
                    if (p2->bal == -1) p1->bal = 1;
                    else  p1->bal = 0;
                    p = p2;
                }
                p->bal = 0;
                h = 0;
            }
        }
    }
    else if (XEquallyKnot(p, key)) {
        // Проверяем, существует ли уже teacher_name в хеш-таблице
        Key* existingKey = p->hashTable->search_for_hash(key);
        if (existingKey == nullptr) { // Если teacher_name не найдено, добавляем его
            Key* keyForHashTable = new Key();
            keyForHashTable = key;
            p->hashTable->insert(keyForHashTable);
            flag=true;
        }

        else
        {
            flag = false;
        }
        h=0;
    }
    if (flag){
        return true;
    }
    else{
        return false;
    }
}


int SearchPerOnceBool(Knot*& root, std::string discipline_name, std::string teacher_name,
    int BB, int CC, int DD, int day, int month, int year, int tableSize) {
    Key* key = new Key();
    key->direction_code.BB = BB;
    key->direction_code.CC = CC;
    key->direction_code.DD = DD;
    key->date.day = day;
    key->date.month = month;
    key->date.year = year;
    key->discipline_name = discipline_name;
    key->teacher_name = teacher_name;

    bool h = false;
    if(SearchBool(key, 0, root, h, tableSize)){
        return 1;
    }
    else{
        return 0;
    }
}

void BalanceL(Knot*& p, bool& h) {
    Knot* p1, * p2;
    if (p->bal == -1) p->bal = 0;
    else if (p->bal == 0) { p->bal = 1; h = false; }
    else {
        p1 = p->right;
        if (p1->bal >= 0) {
            p->right = p1->left;
            p1->left = p;
            if (p1->bal == 0) { p->bal = 1; p1->bal = -1; h = false; }
            else { p->bal = 0; p1->bal = 0; }
            p = p1;
        }
        else {
            p2 = p1->left;
            p1->left = p2->right;
            p2->right = p1;
            p->right = p2->left;
            p2->left = p;
            if (p2->bal == 1) p->bal = -1; else p->bal = 0;
            if (p2->bal == -1) p1->bal = 1; else p1->bal = 0;
            p = p2;
            p2->bal = 0;
        }
    }
}

void BalanceR(Knot*& p, bool& h) {
    Knot* p1, * p2;
    if (p->bal == 1) p->bal = 0;
    else if (p->bal == 0) { p->bal = -1; h = false; }
    else {
        p1 = p->left;
        if (p1->bal <= 0) {
            p->left = p1->right;
            p1->right = p;
            if (p1->bal == 0) { p->bal = -1; p1->bal = 1; h = false; }
            else { p->bal = 0; p1->bal = 0; }
            p = p1;
        }
        else {
            p2 = p1->right;
            p1->right = p2->left;
            p2->left = p1;
            p->left = p2->right;
            p2->right = p;
            if (p2->bal == -1) p->bal = 1; else p->bal = 0;
            if (p2->bal == 1) p1->bal = -1; else p1->bal = 0;
            p = p2;
            p2->bal = 0;
        }
    }
}

void del(Knot*& r, bool& h) {
    if (r->right != nullptr) {
        del(r->right, h);
        if (h) BalanceR(r, h);
    }
    else {
        q->key = r->key;
        q->headp = r->headp;
        q = r;
        r = r->left;
        h = true;
    }
}

bool compare(Key* key, Knot* p) {
    return ((key->date.day == p->key->date.day) and (key->date.month == p->key->date.month) and (key->date.year == p->key->date.year)
        and (key->direction_code.BB == p->key->direction_code.BB) and (key->direction_code.CC == p->key->direction_code.CC)
        and (key->direction_code.DD == p->key->direction_code.DD) and (key->discipline_name == p->key->discipline_name)
        and (key->teacher_name == p->key->teacher_name));
}

bool flag_for_del = false;
bool DeleteBool(Key* key, int y, Knot*& p, bool& h)
{
    if (p == nullptr) {
        std::cout << "Ключа не существует" << std::endl;
        flag_for_del = false;
    }
    else if ((XEquallyKnot(p, key)) and (p->hashTable->search_for_hash(key) != nullptr) and p->hashTable->countElements() > 1)
    {
        if(p->hashTable->remove(key))
        {
            flag_for_del = true;
        }
        else
        {
            flag_for_del = false;
        }
    }
    else if ((XEquallyKnot(p, key)) and (!p->hashTable->search_for_hash(key))) {
        std::cout << "Номера строки не существует" << std::endl;
        flag_for_del = false;
    }
    else if (KeyAboveX(p, key))
    {
        DeleteBool(key, y, p->left, h);
        if (h) BalanceL(p, h);
    }
    else if (KeyBelowX(p, key))
    {
        DeleteBool(key, y, p->right, h);
        if (h) BalanceR(p, h);
    }
    else if (((key->date.day == p->key->date.day) and (key->date.month == p->key->date.month) and (key->date.year == p->key->date.year) and p->hashTable->countElements()==1
             and ((key->direction_code.BB == p->key->direction_code.BB) and (key->direction_code.CC == p->key->direction_code.CC) and (key->direction_code.DD == p->key->direction_code.DD))
             and (key->teacher_name == p->key->teacher_name)) or ((p->hashTable->search_for_hash(key) != nullptr) and (p->hashTable->countElements() == 1))) {
            q = p;
            if (q->right == nullptr) { p = q->left; h = true;flag_for_del = true; }
            else if (q->left == nullptr) { p = q->right; h = true; flag_for_del = true;}
            else
            {
                del(q->left, h);
                if (h) BalanceL(p, h);
                 flag_for_del = true;
            }

    }
    if (flag_for_del){
        return true;
    }
    else
    {
        return false;
    }
}

int DeletePerOnceBool(Knot*& root, std::string discipline_name, std::string teacher_name,
    int BB, int CC, int DD, int day, int month, int year) {
    Key* key = new Key();
    key->direction_code.BB = BB;
    key->direction_code.CC = CC;
    key->direction_code.DD = DD;
    key->date.day = day;
    key->date.month = month;
    key->date.year = year;
    key->discipline_name = discipline_name;
    key->teacher_name = teacher_name;

    bool h = false;
    if(DeleteBool(key, 0, root, h)){
        return 1;
    }
    else{
        return 0;
    }
}

void Delete(Key* key, int y, Knot*& p, bool& h)
{
    if (p == nullptr) {
        std::cout << "Ключа не существует" << std::endl;
    }
    else if ((XEquallyKnot(p, key)) and (p->hashTable->search_for_hash(key) != nullptr) and p->hashTable->countElements() > 1)
    {
        p->hashTable->remove(key);
    }
    else if ((XEquallyKnot(p, key)) and (!p->hashTable->search_for_hash(key))) {
        std::cout << "Номера строки не существует" << std::endl;
    }
    else if (KeyAboveX(p, key))
    {
        Delete(key, y, p->left, h);
        if (h) BalanceL(p, h);
    }
    else if (KeyBelowX(p, key))
    {
        Delete(key, y, p->right, h);
        if (h) BalanceR(p, h);
    }
    else if (compare(key, p)) {
            q = p;
            if (q->right == nullptr) { p = q->left; h = true; }
            else if (q->left == nullptr) { p = q->right; h = true; }
            else {
                del(q->left, h);
                if (h) BalanceL(p, h);

        }
    }
}



bool Duplicate(Knot* p, Key* key) {
    if (p == nullptr)
    {
        return 0;
    }
    else if (KeyAboveX(p, key))
    {
        Duplicate(p->left, key);
    }
    else if (KeyBelowX(p, key))
    {
        Duplicate(p->right, key);
    }
    else if (XEquallyKnot(p, key))
    {
        print(p->headp);
        return 1;
    }
}

std::string Collection_date(Knot* p) {
    std::string s;
    s += std::to_string(p->key->date.day) + ".";
    s += std::to_string(p->key->date.month) + ".";
    s += std::to_string(p->key->date.year);
    return s;
}

std::string Collection_date_d(Date& date) {
    std::ostringstream oss; // Declare std::ostringstream
    oss << std::setfill('0') << std::setw(2) << date.day << "."
        << std::setfill('0') << std::setw(2) << date.month << "."
        << date.year;
    return oss.str(); // Convert stream to string
}

std::string Collection_direction_code(Direction_code& direction_code) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << direction_code.BB << "."
        << std::setfill('0') << std::setw(2) << direction_code.CC << "."
        << std::setfill('0') << std::setw(2) << direction_code.DD;
        return oss.str();
}


inline std::string StringTo(HashNode* p){
    std::string DirecCode = Collection_direction_code(p->key->direction_code);
    std::string Dispil_name = p->key->discipline_name;
    std::string Teacher_Sur = p->key->teacher_name;
    std::string date = Collection_date_d(p->key->date);
    return DirecCode+" "+Dispil_name+" "+Teacher_Sur+" "+date;
}

void Print(Knot* t, int h) { //Распечатать дерево
    if (t != nullptr) {
        Print(t->left, h + 4);
        for (int i = 1; i < h; i++) {
            std::cout << "  ";
        }
        std::cout << Collection_date(t) << "  ";
        std::cout << std::endl;
        Print(t->right, h + 4);
    }
}

void PostOrder(Knot* p) {
    if (p != nullptr) {
        PostOrder(p->left);
        PostOrder(p->right);
        std::cout << Collection_date(p) << " ";
        print(p->headp);
    }
}

void ClearMemory(Knot*& p) {
    if (p == nullptr) {
        return;
    }
    ClearMemory(p->left);
    ClearMemory(p->right);
    print(p->headp);
    delete p;
    p = nullptr;
}
int flag_for_add_message = 0;
void SearchFromFile(Knot*& p, const std::string& filename, int tableSize) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;

        return;
    }

    std::string direction_code, discipline_name, teacher_name, date_str;
    int BB, CC, DD, y = 1;
    char dot; // для чтения точек
    bool h;
    std::string coll;
    flag_for_add_message = 1;
    while (file >> BB >> dot >> CC >> dot >> DD >> discipline_name >> teacher_name >> date_str) {
        Key* key = new Key();
        key->direction_code.BB = BB;
        key->direction_code.CC = CC;
        key->direction_code.DD = DD;
        key->discipline_name = discipline_name;
        key->teacher_name = teacher_name;
        key->date = stringToDate(date_str);
        Search(key, y, p, h, tableSize);
        y++;
        flag_for_add_message = 2;
    }
}


void DeleteFromFile(Knot*& p, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        return;
    }
    std::string discipline_name, teacher_name, date_str;
    int BB, CC, DD, y = 1;
    char dot; // для чтения точек
    bool h;
    std::string coll;

    while (file >> BB >> dot >> CC >> dot >> DD >> discipline_name >> teacher_name >> date_str) {
        Key* key = new Key();
        flag_for_del = false;
        key->direction_code.BB = BB;
        key->direction_code.CC = CC;
        key->direction_code.DD = DD;
        key->discipline_name = discipline_name;
        key->teacher_name = teacher_name;
        key->date = stringToDate(date_str);
        Delete(key, y, p, h);
        y++;
    }
}

void printToFile(Node* head, std::ofstream& file) {
    Node* current = head;
    if (current != nullptr) {
        do {
            file << current->data << " ";
            current = current->next;
        } while (current != head);

        file << std::endl;
    }
}

void PostOrderToFile(Knot* p, const std::string& filename) {
    std::ofstream file(filename, std::ios::app); // Открываем файл в режиме добавления
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        return;
    }

    if (p != nullptr) {
        PostOrderToFile(p->right, filename);
        PostOrderToFile(p->left, filename);
        file << Collection_date(p) << " ";
        printToFile(p->headp, file); // Используйте printToFile вместо print
    }
}

void PrintToFile(Knot* t, int h, const std::string& filename) {
    std::ofstream file(filename, std::ios::app); // Открываем файл в режиме добавления
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл" << std::endl;
        return;
    }

    if (t != nullptr) {
        PrintToFile(t->left, h + 4, filename);
        for (int i = 1; i < h; i++) {
            file << "  ";
        }
        file << Collection_date(t) << "  ";
        printToFile(t->headp, file);
        PrintToFile(t->right, h + 4, filename);
    }
}














MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , findWin(new findwindow(this))  // Инициализация findWin
    , debugWin(new debugwindow(this))
{
    ui->setupUi(this);

    connect(ui->FindNodeBtn, SIGNAL(clicked()), this, SLOT(FindBtn()));
    connect(this, &MainWindow::ToFindSignal, findWin, &findwindow::findSignal);

    connect(ui->DebugNodeBtn, SIGNAL(clicked()), this, SLOT(DebugBtn()));
    connect(this, &MainWindow::ToDebugSignal, debugWin, &debugwindow::debugSignal);

    connect(ui->InitialButton, SIGNAL(clicked()), this, SLOT(Initial()));
    connect(ui->SaveBtnBtn, SIGNAL(clicked()), this, SLOT(SaveBtn()));
    connect(ui->AddNodeBtn,SIGNAL(clicked()),this,SLOT(AddBtnBtn()));
    connect(ui->DeleteNodeBtn,SIGNAL(clicked()), this, SLOT(DelBtnBtn()));
}

MainWindow::~MainWindow()
{
    delete ui;
}



int countOfline;
int countOfline10;
void MainWindow::startSignal(int string, int filelength)
{

    ui->InitialSpin->setMinimum(string);
    ui->InitialSpin->setMaximum(string);
    countOfline = string;
    countOfline10 = filelength;
}

void MainWindow::Initial()
{
    ui->InitialButton->setEnabled(false);
    ui->InitialSpin->setEnabled(false);
    countOfline = ui->InitialSpin->value();

    SearchFromFile(root, "C:/Course/course_project_end/DataBase.txt", countOfline);
    ui->BaseTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->BaseTable->clearContents();
    ui->BaseTable->setRowCount(0);
    MainWindow::WalkTree(root);
    if (flag_for_add_message == 2)
    {
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Information);
        Warn.setText("Загрузка прошла успешно");
        Warn.exec();

    }
    if (flag_for_add_message == 0)
    {
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Critical);
        Warn.setText("База данных не существует");
        Warn.exec();
    }
    if (flag_for_add_message == 1){
        {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Warning);
            Warn.setText("База данных пуста");
            Warn.exec();
        }
    }
}

void MainWindow::AddRow(HashNode* p){
    ui->BaseTable->insertRow(ui->BaseTable->rowCount());
    int j = ui->BaseTable->rowCount()-1;

    QTableWidgetItem* Direc_Code = new QTableWidgetItem(QString::fromStdString(Collection_direction_code(p->key->direction_code)));
    ui->BaseTable->setItem(j, 0, Direc_Code);

    QTableWidgetItem* Dispil_name = new QTableWidgetItem(QString::fromStdString(p->key->discipline_name));
    ui->BaseTable->setItem(j, 1, Dispil_name);

    QTableWidgetItem* Teacher_Sur = new QTableWidgetItem(QString::fromStdString(p->key->teacher_name));
    ui->BaseTable->setItem(j, 2, Teacher_Sur);

    QTableWidgetItem* item_Date = new QTableWidgetItem(QString::fromStdString(Collection_date_d(p->key->date)));
    ui->BaseTable->setItem(j, 3, item_Date);
}

void MainWindow::WalkTree(Knot* p){
    if (p!=nullptr){
        MainWindow::WalkTree(p->left);
        MainWindow::WalkTree(p->right);
        MainWindow::PrinterHashTable(p->hashTable);
    }
}

void MainWindow::PrinterHashTable(HashTable* hashTable){
    for (int i = 0; i < hashTable->size; i++) {
            HashNode* node = hashTable->table[i];
            std::cout << "Bucket " << i << ": ";
            while (node != nullptr) {
                MainWindow::AddRow(node);
                std::cout << node->key->discipline_name << " " << node->key->date.day << "." << node->key->date.month << "." << node->key->date.year << " " << node->key->direction_code.BB << "."
                << node->key->direction_code.CC << "." << node->key->direction_code.DD << " " << node->key->teacher_name << " "
                << "-> ";
                node = node->next;
            }
            std::cout << "NULL" << std::endl;
        }
}



void MainWindow::SaveBtn(){
    qDebug()<<"Save";
    QFile fileout("C:/Course/course_project_end/DataBase.txt");
    if(fileout.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream writestream(&fileout);
        WalkTree_To_File(root, writestream);
    }
    QMessageBox Warn;
    Warn.setIcon(QMessageBox::Information);
    Warn.setText("Данные сохранены");
    Warn.exec();
    fileout.close();
}



void MainWindow::PrinterHashTable_To_File(HashTable* hashTable, QTextStream &writestream){
    for (int i = 0; i < hashTable->size; i++) {
            HashNode* node = hashTable->table[i];
            std::cout << "Bucket " << i << ": ";
            while (node != nullptr) {
                writestream<<QString::fromStdString(StringTo(node))+'\n';
                std::cout << node->key->discipline_name << " " << node->key->date.day << "." << node->key->date.month << "." << node->key->date.year << " " << node->key->direction_code.BB << "."
                << node->key->direction_code.CC << "." << node->key->direction_code.DD << " " << node->key->teacher_name << " "
                << "-> ";
                node = node->next;
            }
            std::cout << "NULL" << std::endl;
        }
}

void MainWindow::WalkTree_To_File(Knot* p, QTextStream &writestream){
    if (p!=nullptr){
        MainWindow::WalkTree_To_File(p->left, writestream);
        MainWindow::WalkTree_To_File(p->right, writestream);
        MainWindow::PrinterHashTable_To_File(p->hashTable, writestream);
    }
}

void MainWindow::AddBtnBtn()
{
    std::string DirCode = ui->Direction_Code->text().toStdString();
    std::string DispName = ui->Disp_Name->text().toStdString();
    std::string FullName = ui->T_FullName->text().toStdString();
    int day = ui->DateLine->date().day();
    int month = ui->DateLine->date().month();
    int year = ui->DateLine->date().year();

    int status = CheckCorrectField(DirCode,DispName,FullName);
    if (status ==1)
    {
         Direction_code DirCoder = stringToDirCode(DirCode);
        if(SearchPerOnceBool(root,DispName,FullName,DirCoder.BB, DirCoder.CC, DirCoder.DD,day,month,year, countOfline))
        {
            ui->BaseTable->clearContents();
            ui->BaseTable->setRowCount(0);
            WalkTree(root);
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Information);
            Warn.setText("Данные добавлены");
            Warn.exec();
        }
        else
        {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Critical);
            Warn.setText("Такая комбинация уже имеется в базе");
            Warn.exec();
    }
    }
    else ErrorHandler(status);
}

void MainWindow::DelBtnBtn()
{
    std::string DirCode = ui->Direction_Code->text().toStdString();
    std::string DispName = ui->Disp_Name->text().toStdString();
    std::string FullName = ui->T_FullName->text().toStdString();
    int day = ui->DateLine->date().day();
    int month = ui->DateLine->date().month();
    int year = ui->DateLine->date().year();

    int status = CheckCorrectField(DirCode,DispName,FullName);
    if (status ==1)
    {
        flag_for_del = false;
         Direction_code DirCoder = stringToDirCode(DirCode);
        if(DeletePerOnceBool(root,DispName,FullName,DirCoder.BB, DirCoder.CC, DirCoder.DD,day,month,year))
        {
            ui->BaseTable->clearContents();
            ui->BaseTable->setRowCount(0);
            WalkTree(root);
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Information);
            Warn.setText("Данные удалены");
            Warn.exec();
        }
        else
        {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Critical);
            Warn.setText("Такой комбинации нет в базе");
            Warn.exec();
    }
    }
    else ErrorHandler(status);
}

void MainWindow::FindBtn(){
    emit ToFindSignal(root);
    findWin->show();

}

void MainWindow::DebugBtn(){
    emit ToDebugSignal(root);
    debugWin->show();

}
