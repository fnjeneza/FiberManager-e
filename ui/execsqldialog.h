#ifndef EXECSQLDIALOG_H
#define EXECSQLDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFileDialog>

namespace Ui {
class ExecSqlDialog;
}

class ExecSqlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExecSqlDialog(QWidget *parent = 0);
    ~ExecSqlDialog();

private slots:
    void on_browse_clicked();

    void on_connect_clicked();

    void on_execute_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::ExecSqlDialog *ui;
    QSqlDatabase db;
};

#endif // EXECSQLDIALOG_H
