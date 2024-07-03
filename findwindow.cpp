#include "findwindow.h"
#include "ui_findwindow.h"
#include <iomanip>
#include <vector>
#include <stack>
#include <Knot.h>
#include <QLineEdit>
#include <QMessageBox>
#include <regex>

findwindow::findwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::findwindow)
{
    ui->setupUi(this);
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(findhash()));
}

findwindow::~findwindow()
{
    delete ui;
}

void findwindow::findSignal(Knot* root)
{
    this->m_root = root;
}

void printHashTable(HashTable* hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        HashNode* node = hashTable->table[i];
        std::cout << "Bucket " << i << ": ";
        while (node != nullptr) {
            std::cout << node->key->discipline_name << " " << node->key->date.day << "." << node->key->date.month << "." << node->key->date.year << " " << node->key->direction_code.BB << "." << node->key->direction_code.CC << "." << node->key->direction_code.DD << " " << node->key->teacher_name << " -> ";
            node = node->next;
        }
        std::cout << "NULL" << std::endl;
    }
}

void printHashTablesInTree(Knot* root) {
    if (root == nullptr) {
        return;
    }

    printHashTablesInTree(root->left);

    std::cout << "Hash table for node with date: " << root->key->date.day << "-" << root->key->date.month << "-" << root->key->date.year << std::endl;
    printHashTable(root->hashTable);
    std::cout << std::endl;

    printHashTablesInTree(root->right);
}

bool isDateWithinRange(const Date& date, const Date& start_date, const Date& end_date) {
    if (date.year < start_date.year || date.year > end_date.year) {
        return false;
    }
    if (date.year == start_date.year) {
        if (date.month < start_date.month || (date.month == start_date.month && date.day < start_date.day)) {
            return false;
        }
    }
    if (date.year == end_date.year) {
        if (date.month > end_date.month || (date.month == end_date.month && date.day > end_date.day)) {
            return false;
        }
    }
    return true;
}

std::string Collection_date_d_d(Date& date) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << date.day << "." << std::setfill('0') << std::setw(2) << date.month << "." << date.year;
    return oss.str();
}

std::string Collection_direction_code_d(Direction_code& direction_code) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << direction_code.BB << "." << std::setfill('0') << std::setw(2) << direction_code.CC << "." << std::setfill('0') << std::setw(2) << direction_code.DD;
    return oss.str();
}

void findwindow::AddRow(const QString& date, const QString& bucket_index, const QString& direc_code, const QString& dispil_name, const QString& teacher_sur) {
    ui->BaseTable->insertRow(ui->BaseTable->rowCount());
    int j = ui->BaseTable->rowCount() - 1;

    QTableWidgetItem* item_Date = new QTableWidgetItem(date);
    ui->BaseTable->setItem(j, 0, item_Date);

    if (!bucket_index.isEmpty()) {
        QTableWidgetItem* item_BucketIndex = new QTableWidgetItem(bucket_index);
        ui->BaseTable->setItem(j, 1, item_BucketIndex);
    }

    if (!direc_code.isEmpty()) {
        QTableWidgetItem* Direc_Code = new QTableWidgetItem(direc_code);
        ui->BaseTable->setItem(j, 2, Direc_Code);
    }

    if (!dispil_name.isEmpty()) {
        QTableWidgetItem* Dispil_name = new QTableWidgetItem(dispil_name);
        ui->BaseTable->setItem(j, 3, Dispil_name);
    }

    if (!teacher_sur.isEmpty()) {
        QTableWidgetItem* Teacher_Sur = new QTableWidgetItem(teacher_sur);
        ui->BaseTable->setItem(j, 4, Teacher_Sur);
    }
}
bool flag_for_message = false;
std::vector<Key*> findwindow::SearchByDateRange(Knot* root, Date start_date, Date end_date, const std::string& disciplin_name) {
    std::vector<Key*> result;
    std::vector<Knot*> stack;
    Knot* current = root;
    int search_steps = 0;
    while (current || !stack.empty()) {
        while (current) {
            stack.push_back(current);
            current = current->left;
            search_steps++;
        }

        current = stack.back();
        stack.pop_back();

        if (isDateWithinRange(current->key->date, start_date, end_date) and current->hashTable->find(current->key, disciplin_name)) {
            QString dateStr = QString::fromStdString(Collection_date_d_d(current->key->date));
            this->AddRow(dateStr); // Add a row with only the date

            for (int i = 0; i < current->hashTable->size; i++) {
                HashNode* node = current->hashTable->table[i];
                while (node != nullptr) {
                    if (node->key->discipline_name == disciplin_name){
                    QString bucket_index = QString::number(i);
                    QString direc_code = QString::fromStdString(Collection_direction_code_d(node->key->direction_code));
                    QString dispil_name = QString::fromStdString(node->key->discipline_name);
                    QString teacher_sur = QString::fromStdString(node->key->teacher_name);
                    this->AddRow("", bucket_index, direc_code, dispil_name, teacher_sur);
                    flag_for_message = true;
                    }

                    node = node->next;
                }
            }
            std::cout << "number of search steps: " <<search_steps << std::endl;
            result.push_back(current->key);
        }

        current = current->right;
    }
    if (flag_for_message){
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Information);
        Warn.setText("Данные найдены");
        Warn.exec();
        flag_for_message = false;
    }
    else{
        QMessageBox Warn;
        Warn.setIcon(QMessageBox::Critical);
        Warn.setText("Данные не найдены");
        Warn.exec();
    }

    return result;
}

inline bool isValidDirectionCodeF(const std::string &str) {
    std::regex pattern(R"(^\d{1}[1-9]\.\d{1}[1-9]\.\d{1}[1-9]$)");
    return std::regex_match(str, pattern);
}

inline bool isValidDisciplineNameF(const std::string &input) {
    std::regex pattern(R"(^[A-Za-z]+(?:_[A-Za-z]+)*$)");
    return std::regex_match(input, pattern);
}

inline bool isValidFullNameF(const std::string &input) {
    std::regex pattern(R"(^[A-Z][a-z]*_[A-Z][a-z]*_[A-Z][a-z]*$)");
    return std::regex_match(input, pattern);
}

inline int CheckCorrectFieldF(std::string DirCode, std::string DispName, std::string FullName){
    if (!isValidDirectionCodeF(DirCode)){
        return -1;
    }else if (!isValidDisciplineNameF(DispName)){
        return -2;
    }else if (!isValidFullNameF(FullName)){
        return -3;
    }else return 1;
}

inline void ErrorHandlerF(int status){
    QMessageBox Warn;
    Warn.setIcon(QMessageBox::Critical);
switch (status){
    case -1:Warn.setText("Неправильный формат кода направления. (AA.BB.CC)");break;
    case -2:Warn.setText("Неправильный формат названия направления.(Software_Engineering)");break;
    case -3:Warn.setText("Неправильный формат ФИО. (Ivanov_Ivan_Ivanovich)");break;
}
    Warn.exec();
}

int date_check = 0;
void findwindow::findhash() {
    Date date_from;
    Date date_to;


    date_from.day = ui->dateEdit->date().day();
    date_from.month = ui->dateEdit->date().month();
    date_from.year = ui->dateEdit->date().year();
    date_to.day = ui->dateEdit_2->date().day();
    date_to.month = ui->dateEdit_2->date().month();
    date_to.year = ui->dateEdit_2->date().year();
    std::string discip_name = ui->lineEdit_4->text().toStdString();
    if (date_from.year > date_to.year){
        date_check = 1;
    }
    if ((date_from.year == date_to.year) and (date_from.month > date_to.month)){
        date_check = 2;
    }
    if ((date_from.year == date_to.year) and (date_from.month == date_to.month) and (date_from.day > date_to.day)){
        date_check = 3;
    }
    ui->BaseTable->setRowCount(0);
    int status = CheckCorrectFieldF("02.03.04",discip_name,"A_A_A");

    if (status ==1)
    {
        if (date_check == 0)
        {
            std::vector<Key*> keysInRange = SearchByDateRange(this->m_root, date_from, date_to, discip_name);
                for (const auto& key : keysInRange)
                {
                    std::cout << "Found key with date: " << key->date.year << "-" << key->date.month << "-" << key->date.day << std::endl;
                }
        }
        if (date_check == 1)
        {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Critical);
            Warn.setText("Ошибка ввода дат. Год начала поиска больше года окончания поиска");
            Warn.exec();
            date_check = 0;
        }
        if (date_check == 2)
        {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Critical);
            Warn.setText("Ошибка ввода дат. Месяц начала поиска больше месяца окончания поиска");
            Warn.exec();
            date_check = 0;
        }
        if (date_check == 3)
        {
            QMessageBox Warn;
            Warn.setIcon(QMessageBox::Critical);
            Warn.setText("Ошибка ввода дат. День начала поиска больше дня окончания поиска");
            Warn.exec();
            date_check = 0;
        }
    }
    else
    {
        ErrorHandlerF(status);
    }
}



