#include "mainfm.h"
#include "ui_mainfm.h"

MainFM::MainFM(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainFM)
{
    ui->setupUi(this);
}

MainFM::~MainFM()
{
    delete ui;
    delete config;
}

void MainFM::on_actionBdd_triggered()
{
    config = new Config(ui->centralwidget);
    config->show();
}

