#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

#include "handler/confighandler.h"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(QWidget *parent = 0);
    ~ConfigDialog();

    void remove(QString project_id);

    QSqlDatabase getPostgresDatabase() const;
    void setPostgresDatabase(const QSqlDatabase &value);

private slots:
    void on_connect_clicked();

    void on_newConfig_clicked();

    void on_save_clicked();

    void on_deleteConfig_clicked();

    void on_browseOptique_clicked();

    void on_editConfig_clicked();

private:
    Ui::ConfigDialog *ui;
    ConfigHandler *configHandler;
    Parameters param;

    QSqlDatabase postgresDatabase;
};

#endif // CONFIGDIALOG_H
