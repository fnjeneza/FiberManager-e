#include "dialog.h"
#include "ui_dialog.h"
#include "QFileDialog"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{

    ui->lineEdit->setText(QFileDialog::getOpenFileName(this,"Fichier base Circet","", "Base mdb (*.mdb)"););
}

void Dialog::on_pushButton_2_clicked()
{
    ui->lineEdit_2->setText(QFileDialog::getOpenFileName(this,"Fichier base Free","", "Base mdb (*.mdb)"););
}

void Dialog::on_pushButton_3_clicked()
{
    QSqlDatabase db1 = QSqlDatabase::addDatabase("QPSQL", "1");
    QSqlDatabase db2 = QSqlDatabase::addDatabase("QODBC", "2");
    QString q = "select CODE from site";
    QSqlQuery
}
