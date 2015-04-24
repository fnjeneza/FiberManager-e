#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    //initiate config handler
    configHandler = new ConfigHandler();
    ui->projectId->addItems(configHandler->retrieveAllProjectId());
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
    delete configHandler;
}

void ConfigDialog::on_connect_clicked()
{
    QString projectId=ui->projectId->currentText();

    if(projectId.isEmpty()){
        QMessageBox::warning(this, "Erreur", "Connexion impossible, le nom est vide");
        return;
    }
    param = configHandler->getConfiguration(projectId);

    postgresDatabase = QSqlDatabase::addDatabase("QPSQL", "test");
    postgresDatabase.setHostName(param.getHost());
    postgresDatabase.setDatabaseName(param.getBase());
    postgresDatabase.setUserName(param.getUserName());
    postgresDatabase.setPassword(param.getPassword());


    if(!postgresDatabase.open()){
        QMessageBox::warning(this, "Erreur de connexion", postgresDatabase.lastError().text());
        return;
    }
        QMessageBox::information(this, "Connexion", "Connexion reussie");
}

void ConfigDialog::on_newConfig_clicked()
{
     ui->widgetConnexion->setEnabled(true);
}

void ConfigDialog::on_save_clicked()
{
    Parameters newConfig =  Parameters(ui->id->text(),
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
    if(configHandler->exists(newConfig)){
        configHandler->update(newConfig);
        return;
    }

    configHandler->addConfig(newConfig);
    ui->projectId->clear();
    ui->projectId->addItems(configHandler->retrieveAllProjectId());

}

void ConfigDialog::on_deleteConfig_clicked()
{
    Parameters config = Parameters();
    config.setProjectId(ui->projectId->currentText());
    configHandler->removeConfig(config);
    ui->projectId->clear();
    ui->projectId->addItems(configHandler->retrieveAllProjectId());
}

void ConfigDialog::on_browseOptique_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Optique"), ".", tr("Base mdb (*.mdb)"));
    if(fileName.isEmpty()){
        return;
    }
    ui->optique->setText(fileName);
}

void ConfigDialog::on_editConfig_clicked()
{
    Parameters config = configHandler->getConfiguration(ui->projectId->currentText());
    ui->id->setText(config.getProjectId());
    ui->nro->setText(config.getNro());
    ui->plaque->setText(config.getPlaque());
    ui->ext->setText(config.getExt());
    ui->databaseName->setText(config.getBase());
    ui->host->setText(config.getHost());
    ui->port->setText(config.getPort());
    ui->userName->setText(config.getUserName());
    ui->pwd->setText(config.getPassword());
    ui->optique->setText(config.getOptique());
    ui->infra->setText(config.getInfra());
    ui->site->setText(config.getSite());
}
QSqlDatabase ConfigDialog::getPostgresDatabase() const
{
    return postgresDatabase;
}

void ConfigDialog::setPostgresDatabase(const QSqlDatabase &value)
{
    postgresDatabase = value;
}

