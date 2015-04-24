#include "execsqldialog.h"
#include "ui_execsqldialog.h"

ExecSqlDialog::ExecSqlDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExecSqlDialog)
{
    ui->setupUi(this);
}

ExecSqlDialog::~ExecSqlDialog()
{
    delete ui;
}

void ExecSqlDialog::on_browse_clicked()
{
     ui->path->setText(QFileDialog::getOpenFileName(this, "Base", "", "Base mdb(*.mdb)"));
}

void ExecSqlDialog::on_connect_clicked()
{
    if(db.isOpen()){
        db.close();
        db.removeDatabase("execsql");
    }

    db = QSqlDatabase::addDatabase("QODBC", "execsql");
    db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+ui->path->text());
    if(!db.open()){
        QMessageBox::warning(this, "Erreur connection", db.lastError().text());
    }
    QMessageBox::warning(this, "Connection", "Connection réussie");
}

void ExecSqlDialog::on_execute_clicked()
{
    if(ui->sql->toPlainText().isEmpty()){
        return;
    }
    QStringList queries = ui->sql->toPlainText().split(";");
    int nbQuery=0;
    for(int i=0; i<queries.length(); i++){
        if(queries.at(i).isEmpty()){
            continue;
        }
        QSqlQuery q = db.exec(queries.at(i));
        if(!q.isActive()){
            QMessageBox::warning(this,"Erreur sql",q.lastError().text());
            return;
        }
        nbQuery++;
    }
    if(nbQuery>=1){
        ui->label->setText(QString::number(nbQuery)+" requête(s) executée(s)");
    }
}

void ExecSqlDialog::on_buttonBox_accepted()
{
    if(db.isOpen()){
        db.close();
    }
}

void ExecSqlDialog::on_buttonBox_rejected()
{
    if(db.isOpen()){
        db.close();
    }
}
