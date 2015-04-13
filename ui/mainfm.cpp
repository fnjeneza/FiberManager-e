#include "mainfm.h"
#include "ui_mainfm.h"

MainFM::MainFM(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainFM)
{
    ui->setupUi(this);
    prefSet=false;

}

MainFM::~MainFM()
{
    if(ui!=NULL){
        delete ui;
    }
    if(config!=NULL){
        delete config;
    }
    if(mdb!=NULL){
        delete mdb;
    }
    if(psql!=NULL){
        delete psql;
    }
}

void MainFM::on_actionBdd_triggered()
{
    config = new Config(ui->centralwidget);
    config->show();
}


void MainFM::on_actionSites_triggered()
{
    QString message = "Vous êtes sur le point d'importer les sites de la base GIS vers la base Optique"
                      "Ceci entrainera une vérification des sites...";

}

void MainFM::on_actionFLR_triggered()
{

    QFile csv(QDir::tempPath()+"/flr.csv");
    csv.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&csv);
    out<<"hexacle;adresse;nb_bal;phd;parcelle;zone;type_habitat;type_chausse;anc_chaussee;type_trottoir;anc_trottoir;gestionnaire;nro;poche;boite;adduction;concessionnaire;pm;lr_pm;commentaire\n";
    csv.flush();

    QProgressDialog progress(this);
    progress.setLabelText("Export FLR...");
    progress.setModal(true);

//    if(!prefSet){
//        config = new Config(ui->centralwidget);
//        config->show();
//        pref = config->getConfig();
//        prefSet=true;
//    }
//    psql->setHost(pref.getHost());
    //psql->setPort(pref.getPort());
//    psql->setNro(pref.getNro());
//    psql->setDatabaseName(pref.getBase());
//    psql->setUserName(pref.getUserName());
//    psql->setPassword(pref.getPassword());


    psqlConnect();

    QStringList adresses = psql->retrieveAdresses();
    int total = adresses.count();
    progress.setMaximum(total);

    for(int i=0; i<total;i++){
        progress.setValue(i);
        if(progress.wasCanceled()){
            progress.close();
            break;
        }
        QStringList value = adresses.at(i).split(";");
        QString hexacle = value.at(0);
        QString num = value.at(1);
        QString suf = value.at(2);
        QString voie = value.at(3);

        QStringList lr = psql->lr(hexacle,num,suf,voie);
        QString habitat = lr.at(2).toInt()>1?"COLL":"INDIV";
        lr.insert(9,pref.getNro());
        lr.insert(4,habitat);
        lr.insert(4, "zone");
        lr.insert(3,"PHD");

        out<<lr.join(";");
        csv.flush();
    }
    progress.setValue(total);
    csv.close();
}



void MainFM::on_actionCasage_triggered()
{
    psqlConnect();
    psql->adressesWithoutHexacode();
    psql->invalidADresses();

}

void MainFM::psqlConnect(){
    if(psql==NULL){

        psql = new PsqlDatabase();
        psql->setHost("192.168.0.126");
        psql->setDatabaseName("PLA13_038");
        psql->setUserName("be_free");
        psql->setPassword("123456");
        psql->connect();
    }
}
