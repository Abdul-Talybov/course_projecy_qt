#include "debugwindow.h"
#include "ui_debugwindow.h"
#include <iomanip>
debugwindow::debugwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::debugwindow)
{
    ui->setupUi(this);
}

debugwindow::~debugwindow()
{
    delete ui;
}

void debugwindow::debugSignal(Knot* root)
{
    this->d_root = root;    
    ui->textEdit->clear();
    Print_debug(root, 0, ui->textEdit);
    ui->BaseTable->setRowCount(0);
    printHashTablesInTree(root);
}

std::string Collection_date_debug(Date& date) {
    std::ostringstream oss; // Declare std::ostringstream
    oss << std::setfill('0') << std::setw(2) << date.day << "."
        << std::setfill('0') << std::setw(2) << date.month << "."
        << date.year;
    return oss.str(); // Convert stream to string
}

std::string Collection_direction_code_debug(Direction_code& direction_code) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << direction_code.BB << "." << std::setfill('0') << std::setw(2) << direction_code.CC << "." << std::setfill('0') << std::setw(2) << direction_code.DD;
    return oss.str();
}

void debugwindow::Print_debug(Knot* t, int h, QTextEdit* textEdit, bool isRightChild) {
    if (t != nullptr) {
        // Печать левого поддерева
        Print_debug(t->left, h + 10, textEdit, false);

        // Печать текущего узла с отступом
        QString indentation(h, ' ');
        QString nodeText = QString::fromStdString(Collection_date_debug(t->key->date));
        if (isRightChild) {
            textEdit->append(indentation + "└── " + nodeText);  // Стрелка для правого поддерева
        } else {
            textEdit->append(indentation + "┎── " + nodeText);  // Если нет правого поддерева
        }

        // Печать правого поддерева
        Print_debug(t->right, h + 10, textEdit, true);
    }
}

void debugwindow::AddRow_D(const QString& date, const QString& bucket_index, const QString& direc_code, const QString& dispil_name, const QString& teacher_sur) {
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

void debugwindow::printHashTable(HashTable* hashTable, const std::string& date) {
    for (int i = 0; i < hashTable->size; i++) {
        HashNode* node = hashTable->table[i];


        while (node != nullptr) {
            debugwindow::AddRow_D("",
                       QString::number(i),
                       QString::fromStdString(Collection_direction_code_debug(node->key->direction_code)),
                       QString::fromStdString(node->key->discipline_name),
                       QString::fromStdString(node->key->teacher_name));
            node = node->next;
        }
    }
}

void debugwindow::printHashTablesInTree(Knot* root) {
    if (root == nullptr) {
        return;
    }

    printHashTablesInTree(root->left);

    std::string date = Collection_date_debug(root->key->date);
    debugwindow::AddRow_D(QString::fromStdString(date), "", "", "", ""); // Add row with date
    printHashTable(root->hashTable,  date);

    printHashTablesInTree(root->right);
}
