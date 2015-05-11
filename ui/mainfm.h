#ifndef MAINFM_H
#define MAINFM_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QMessageBox>

#include "configdialog.h"
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
    bool connect();
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

    void on_actionMat_riel_d_ploy_triggered();

private:
    Ui::MainFM *ui;
    ConfigDialog *config=NULL;
    MdbHandler *mdb=NULL;
    PsqlDatabase *psql=NULL;
    //Parameters *configDao=NULL;
    Parameters param;
    ExecSqlDialog *sqlDlg=NULL;
    DOEGCdlg *doegc=NULL;

    QDialog *dlg;
    QString shapePlaque;
    QString numSect;
};

#endif // MAINFM_H
