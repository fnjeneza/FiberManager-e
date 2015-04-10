#ifndef MAINFM_H
#define MAINFM_H

#include <QMainWindow>

#include "config.h"

namespace Ui {
class MainFM;
}

class MainFM : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainFM(QWidget *parent = 0);
    ~MainFM();


private slots:
    void on_actionBdd_triggered();

private:
    Ui::MainFM *ui;
    Config *config;
};

#endif // MAINFM_H
