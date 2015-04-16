#include "mainfm.h"
#include "ui_mainfm.h"

MainFM::MainFM(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainFM)
{
    ui->setupUi(this);
    prefSet=false;
    pref.setHost("10.0.0.254");
    pref.setUserName("be_free");
    pref.setPassword("123456");
    pref.setBase("PLA13_038");
    pref.setPlaque("PLA13_038");
    pref.setSite("T:\\test\\Bases\\Site.mdb");
    pref.setOptique("T:\\test\\PLA13_038_SLO13\\Optique\\Bases\\Secteur.mdb");
    pref.setInfra("T:\\test\\PLA13_038_SLO13\\Infra\\Bases\\Secteur.mdb");
    pref.setNro("SLO13");

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
   psqlConnect();
   mdb->connect();
   psql->connect();
   QStringList hexacles = psql->getAllHexacles();

   psql->getSite("1321522ZNP");
   qDebug()<<mdb->isZoneAvant("29","", "RUE SAINTE VICTORINE");




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
//    psql->setPort(pref.getPort());
//    psql->setNro(pref.getNro());
//    psql->setDatabaseName(pref.getBase());
//    psql->setUserName(pref.getUserName());
//    psql->setPassword(pref.getPassword());


    psqlConnect();

    psql->connect();
    mdb->connect();



    QStringList hexacles = psql->getAllHexacles();
    int nb_site = hexacles.count();
    progress.setMaximum(nb_site);

    for(int i=0; i<nb_site;i++){

        progress.setValue(i);
        if(progress.wasCanceled()){
            progress.close();
            break;
        }
        QString hexacle = hexacles.at(i);

        if(hexacle.isEmpty()){
            continue;
            QMessageBox::warning(this, "Export interrompu", "L'export ne peut être finalisé.\nMerci de faire l'audit casage");
            break;
        }
        QStringList values = psql->getSite(hexacle);
        QString num = values.at(0);
        QString suf = values.at(1);
        QString adresse=suf.isEmpty()?num:num+" "+suf;
        QString voie = values.at(2);
        adresse =num+" "+voie+" "+values.at(3)+" "+values.at(4);
        QString bal = values.at(5);

        QString boite = values.at(6);
        QString adduction = values.at(7);
        if(adduction.isEmpty()){
            adduction="GC";
        }
        QString concessionnaire = values.at(8);
        if(concessionnaire.isEmpty()){
            concessionnaire="FT";
        }

        QString habitat = bal.toInt()>1?"COLL":"INDIV";
        QString nro = pref.getNro();
        QString zone = "Zone Arriere";
        if(mdb->isZoneAvant(num, suf, voie)){
            zone= "Zone Avant";
        }

        QString commentaire = values.at(11);
        QString parcelle = psql->getParcelle(hexacle);
        QString densite = "PHD";
        QStringList voirie = psql->infoRue(voie);
        QString type_ch = voirie.at(0);
        QString anc_ch = voirie.at(1);
        QString type_tr = voirie.at(2);
        QString anc_tr = voirie.at(3);
        QString gestionnaire = voirie.at(4);

        QString poche = psql->getPoche(hexacle);
        if(poche.isEmpty()){
            parcelle = "HORS PLAQUE";
        }
        if(poche.toLower().compare("inexistant")==0){
            parcelle="ADRESSE INEXISTANTE";
        }
        QString pm = psql->getPm(hexacle);
        if(pm.isEmpty()){
            pm="PAS DE PM;0";
        }
        if(hexacle.toLower().contains("crea")){
            parcelle="CREATION ADRESSE";
        }

        QStringList line;
        line<<"\""+hexacle+"\""
           <<adresse
          <<bal
         <<densite
        <<parcelle
        <<zone
        <<habitat
        <<type_ch
        <<anc_ch
        <<type_tr
        <<anc_tr
        <<gestionnaire
        <<nro
        <<poche
        <<boite
        <<adduction
        <<concessionnaire
        <<pm
        <<commentaire;

        out<<line.join(";")+"\n";
        csv.flush();
    }
    progress.setValue(nb_site);
    csv.close();
}



void MainFM::on_actionCasage_triggered()
{
    psqlConnect();
    psql->connect();

    QProgressDialog dlg(this);
    dlg.setModal(true);
    dlg.setLabelText("Audit casage...");

    QFile csv(QDir::tempPath()+"/audit.csv");
    csv.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&csv);
    csv.flush();

    //retrieve hexacles
    QStringList hexacles = psql->getAllHexacles();
    int nb_sites = hexacles.count();

    dlg.setMaximum(nb_sites);

    //multiple occurance of hexacle
    QStringList multipleHexacle = psql->multipleHexacleOccurence();
    for(int i=0; i<multipleHexacle.length();i++){
        out<<"Hexaclé non unique;"+multipleHexacle.at(i)+"\n";
    }

    QStringList voieChecked;

    for(int i=0; i<nb_sites; i++){
        dlg.setValue(i);

        QString hexacle = hexacles.at(i);
        //check if hexacle is valid
        if(hexacle.isEmpty()){
            QMessageBox::warning(this, "Erreur hexaclé", "Présence d'adresses sans hexaclés.\n Merci de corriger ces erreurs et de ré-auditer");
            break;
        }
        //retrieve site
        QStringList values = psql->getSite(hexacle);

        //check if num is valid
        QString num = values.at(0);
        if(QString::number(num.toInt()).compare(num)!=0){
            out<<"num non valide;\""+hexacle+"\"\n";
        }

        //check if suf is valid
        if(values.at(1).length()>1){
            out<<"suf non valide;\""+hexacle+"\"\n";
        }

        //check if voie is valid
        QString voie = values.at(2);
        if(voie.isEmpty()){
            if(!voieChecked.contains(voie)){
                out<<"voie non valide;"+voie+";\""+hexacle+"\"\n";
                voieChecked<<voie;
            }

        }

        //check if voie is present in Rivoli table
        bool exists = mdb->isVoieExists(voie);
        if(!exists){
            if(!voieChecked.contains(voie)){
                out<<"voie non présente dans rivoli;"+voie+"\n";
                voieChecked<<voie;
            }
        }

        //check if bal > 0
        if(values.at(5).toInt()<=0){
            out<<"Une adresse existante ne peut avoir 0 LR;\""+hexacle+"\"\n";
        }

        //nommage boite rattachement is correct
        QStringList boites = values.at(6).split(",");
        if(boites.at(0).isEmpty()){
            out<<"boite de rattachement non renseigné;\""+hexacle+"\"\n";
            boites.clear();
        }
        for(int i=0; i<boites.count();i++){
            QString boite = boites.at(i);
            int poche = psql->getPoche(hexacle).toInt();
            if(!boite.contains("PDB") && !boite.contains("BPI")){
                out<<"nommage netgeo incorrect;"+boite+";\""+hexacle+"\"\n";
                continue;
            }
            if(boite.contains("PDB")){
                if(boite.length()!=19){
                    out<<"nommage netgeo incorrect;"+boite+";\""+hexacle+"\"\n";
                    continue;
                }
                if(boite.left(14).compare("PDB_"+pref.getPlaque()+"_")!=0){
                    out<<"nommage netgeo incorrect;"+boite+";\""+hexacle+"\"\n";
                    continue;
                }
                if(boite.right(5).left(2).toInt()!=poche){
                    out<<"Incohérence de la poche pour la boite de rattachement et la poche de l'adresse;\""+hexacle+"\"\n";
                }
            }
            if(boite.contains("BPI")){
                if(boite.length()!=24){
                    out<<"nommage netgeo incorrect;"+boite+";\""+hexacle+"\"\n";
                    continue;
                }
                if(boite.left(14).compare("BPI_SIT_"+pref.getNro()+"_")!=0){
                    out<<"nommage netgeo incorrect;"+boite+";\""+hexacle+"\"\n";
                    continue;
                }
                if(boite.right(5).left(2).toInt()!=poche){
                    out<<"Incohérence de la poche pour la boite de rattachement et la poche de l'adresse;\""+hexacle+"\"\n";
                }
            }
        }

        //adduction value, concessionnaire value
        QString adduction = values.at(7).toLower();
        if(!adduction.isEmpty()){
            if(adduction.compare("aerien")!=0 &&
                    adduction.compare("egout")!=0 &&
                    adduction.compare("facade")!=0 &&
                    adduction.compare("gc")!=0){
                out<<"Type d'adduction incorrect, valeur possible(AERIEN, EGOUT, FACADE, GC);"+hexacle+"\n";
            }
        }

        QString concessionnaire = values.at(8).toLower();
        if(concessionnaire.isEmpty()){
            if(concessionnaire.compare("communal")!=0 &&
                    concessionnaire.compare("fi")!=0 &&
                    concessionnaire.compare("ft")!=0 &&
                    concessionnaire.compare("sap")!=0){
                out<<"Concessionnaire incorrect, valeur possible (COMMUNAL, FI, FT, SAP);"+hexacle+"\n";
            }
        }

        csv.flush();
    }
    csv.close();

    psql->multipleHexacleOccurence();

}

void MainFM::psqlConnect(){
    if(psql==NULL){

        psql = new PsqlDatabase();
        psql->setHost(pref.getHost());
        psql->setDatabaseName(pref.getBase());
        psql->setUserName(pref.getUserName());
        psql->setPassword(pref.getPassword());
        //psql->connect();
    }
    if(mdb==NULL){
        mdb=new MdbHandler();
        mdb->setSite(pref.getSite());
        mdb->setOptique(pref.getOptique());
        mdb->setInfra(pref.getInfra());

        //mdb->connect();
    }
}

//bool MainFM::isHexacleValid(QString hexacle){
//    return hexacle.toString().isEmpty()?false:true;

//}

//bool MainFM::isNumValid(QString num){
//    return num.toString().isEmpty()?false:true;
//}

//bool MainFM::isSufValid(QString suf){
//    return suf.toString().isEmpty()?false:true;
//}

//bool MainFM::isVoieValid(QString voie){
//    return voie.toString().isEmpty()?false:true;
//}

void MainFM::on_actionAuditCorolle_triggered()
{
    psqlConnect();
    mdb->connect();

    QProgressDialog progress(this);
    progress.setLabelText("Audit Corolle...");
    progress.setModal(true);

    QFile csv(QDir::tempPath()+"/auditCorolle.csv");
    csv.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&csv);
    csv.flush();

    QStringList noeuds = mdb->getAllOptiqueNoeud();
    int max = noeuds.length();
    progress.setMaximum(max);
    for(int i=0; i<max;i++){
        progress.setValue(i);

        QString noeud = noeuds.at(i);
        QString optiqueAdr = mdb->getOptiqueNoeudAdresse(noeud);
        QStringList chambre = mdb->getAssociatedChambre(noeud);

        if(chambre.isEmpty()){
            out<<"Noeud non associé à l'infra;"+noeud+"\n";
            continue;
        }
        if(!noeud.contains("L") && chambre.length()>1){
            out<<"Multiple association du noeud à l'infra;"+noeud+";";
            for(int k = 0; k<chambre.length();k++){
                out<<chambre.at(k)+",";
            }
            out<<"\n";
            continue;
        }

        QString infraAdr = mdb->getInfraNoeudAdresse(chambre.at(0));
        if(!noeud.contains("L") && infraAdr.compare(optiqueAdr)!=0){
            out<<"Incohérence entre adresse ;"+noeud+" et "+chambre.at(0)+"\n";
        }
    csv.flush();
    }

    csv.close();



}
