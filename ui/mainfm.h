#ifndef MAINFM_H
#define MAINFM_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QMessageBox>

#include "config.h"
#include "handler/database/mdbhandler.h"
#include "handler/database/psqldatabase.h"
#include "execsqldialog.h"
#include "doegcdlg.h"

namespace Ui {
class MainFM;
}

class MainFM : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainFM(QWidget *parent = 0);
    ~MainFM();
    void connect();
    void disconnect();


private slots:
    void on_actionBdd_triggered();

    void on_actionSites_triggered();

    void on_actionFLR_triggered();

    void on_actionCasage_triggered();

    void on_actionAuditCorolle_triggered();

    void on_actionMAJ_Base_CTR_triggered();

    void on_actionLTA_triggered();

    void on_actionA_propos_triggered();

    void on_actionGC_2_triggered();

private:
    Ui::MainFM *ui;
    Config *config=NULL;
    MdbHandler *mdb=NULL;
    PsqlDatabase *psql=NULL;
    FMConfigDAO pref;
    ExecSqlDialog *sqlDlg=NULL;
    DOEGCdlg *doegc=NULL;
    bool prefSet;
    QDialog *dlg;
    QString shapePlaque;
    QString numSect;
};

#endif // MAINFM_H
