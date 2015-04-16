#ifndef MAINFM_H
#define MAINFM_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QMessageBox>

#include "config.h"
#include "handler/database/mdbhandler.h"
#include "handler/database/psqldatabase.h"

namespace Ui {
class MainFM;
}

class MainFM : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainFM(QWidget *parent = 0);
    ~MainFM();
    void psqlConnect();


private slots:
    void on_actionBdd_triggered();

    void on_actionSites_triggered();

    void on_actionFLR_triggered();

    void on_actionCasage_triggered();

    void on_actionAuditCorolle_triggered();

private:
    Ui::MainFM *ui;
    Config *config=NULL;
    MdbHandler *mdb=NULL;
    PsqlDatabase *psql=NULL;
    FMConfigDAO pref;
    bool prefSet;
};

#endif // MAINFM_H
