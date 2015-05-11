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


    accessDatabase.setSite(param.getSite());
    accessDatabase.setOptique(param.getOptique());
    accessDatabase.setInfra(param.getInfra());
    if(!accessDatabase.connect()){
        QMessageBox::warning(this, "Erreur connexion", "Impossible de se connecter vérifier les paramètres de la base corolle");
        return;
    }

    postgisDatabase.setHost(param.getHost());
    postgisDatabase.setDatabaseName(param.getBase());
    postgisDatabase.setUserName(param.getUserName());
    postgisDatabase.setPassword(param.getPassword());
    if(!postgisDatabase.connect()){
        QMessageBox::warning(this, "Erreur connexion", "Impossible de se connecter au serveur postgis");
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
    QString optique = ui->optique->text();
    QStringList arborescence = optique.split("/");
    QStringList tmp;
    for(int i=0; i<arborescence.length()-3; i++){
        tmp<<arborescence.at(i);
    }
    tmp<<"Infra/Bases/Secteur.mdb";
    QString infra = tmp.join("/");
    tmp.clear();
    for(int i=0; i<arborescence.length()-4; i++){
        tmp<<arborescence.at(i);
    }
    tmp<<"Bases/Site.mdb";
    QString site = tmp.join("/");
    Parameters newConfig =  Parameters(ui->id->text(),
                                        ui->nro->text(),
                                        ui->plaque->text(),
                                        ui->ext->text(),
                                        ui->databaseName->text(),
                                        ui->host->text(),
                                        ui->port->text(),
                                        ui->userName->text(),
                                        ui->pwd->text(),
                                        optique,
                                        infra,
                                        site);
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
}
Parameters ConfigDialog::getParam() const
{
    return param;
}

MdbHandler ConfigDialog::getAccessDatabase() const
{
    return accessDatabase;
}
PsqlDatabase ConfigDialog::getPostgisDatabase() const
{
    return postgisDatabase;
}
