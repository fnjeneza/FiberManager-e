#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
    configHandler = new ConfigHandler();
    ui->projectId->addItems(configHandler->retrieveAllProjectId());

}

Config::~Config()
{
    delete ui;
    delete configHandler;
}

void Config::on_connect_clicked()
{

    QString projectId=ui->projectId->currentText();

    if(projectId.isEmpty()){
        qDebug()<<"Connexion impossible, le nom est vide";
        return;
    }
}

FMConfigDAO Config::getConfig(){
    return config;
}


void Config::on_newConfig_clicked()
{
    ui->widgetConnexion->setEnabled(true);
}

void Config::on_save_clicked()
{
    FMConfigDAO newConfig =  FMConfigDAO(ui->id->text(),
                                        ui->nro->text(),
                                        ui->plaque->text(),
                                        ui->ext->text(),
                                        ui->databaseName->text(),
                                        ui->host->text(),
                                        ui->port->text(),
                                        ui->userName->text(),
                                        ui->pwd->text(),
                                        ui->optique->text(),
                                        ui->infra->text(),
                                        ui->site->text());

    configHandler->addConfig(newConfig);
    ui->projectId->clear();
    ui->projectId->addItems(configHandler->retrieveAllProjectId());


}

void Config::on_deleteConfig_clicked()
{
    FMConfigDAO config = FMConfigDAO();
    config.setProjectId(ui->projectId->currentText());
    configHandler->removeConfig(config);
    ui->projectId->clear();
    ui->projectId->addItems(configHandler->retrieveAllProjectId());

}

void Config::on_browseOptique_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Optique"), ".", tr("Base mdb (*.mdb)"));
    ui->optique->setText(fileName);
}
