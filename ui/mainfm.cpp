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
//    pref.setSite("H:\\MarseilleFree2014\\Bases\\Site.mdb");
//    pref.setOptique("H:\\MarseilleFree2014\\PLA13_038_SLO13\\Optique\\Bases\\Secteur.mdb");
//    pref.setInfra("H:\\MarseilleFree2014\\PLA13_038_SLO13\\Infra\\Bases\\Secteur.mdb");
    pref.setSite("T:\\test\\Bases\\Site.mdb");
    pref.setOptique("T:\\test\\PLA13_038_SLO13\\Optique\\Bases\\Secteur.mdb");
    pref.setInfra("T:\\test\\PLA13_038_SLO13\\Infra\\Bases\\Secteur.mdb");
    pref.setNro("SLO13");
    shapePlaque = "plaque";
    numSect = "D5B5";
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
    if(sqlDlg!=NULL){
        delete sqlDlg;
    }
    if(doegc!=NULL){
        delete doegc;
    }
}

void MainFM::on_actionBdd_triggered()
{
    config = new Config(ui->centralwidget);
    config->show();
}


void MainFM::on_actionSites_triggered()
{
   connect();
   mdb->connect();
   psql->connect();

   QProgressDialog progress(this);
   progress.setLabelText("Import de sites...");
   progress.setModal(true);
   progress.setValue(0);

   QStringList hexacles = psql->getAllHexaclesInPlaque(shapePlaque);
   progress.setMaximum(hexacles.length());
   for(int i=0; i<hexacles.length();i++){
       progress.setValue(i);
       if(hexacles.at(i).isEmpty()){
           QMessageBox::warning(this, "Erreur adresse sans hexaclés","Présence d'adresse sans hexaclés, corriger ces erreurs avant import de sites");
           return;
       }
   }


   for(int i=0; i<hexacles.length(); i++){
       progress.setValue(i);
       if(progress.wasCanceled()){
           progress.close();
           break;
       }
       QStringList site = psql->getSite(hexacles.at(i));
       QString recno=mdb->nextRecno("sites");
       QString no = mdb->nextSiteNumber();
       QString num = site.at(0);
       QString suf = site.at(1);
       QString voie = site.at(2);
       QString code_postal = site.at(3);
       QString ville = site.at(4);
       int nbre_foyer = site.at(5).toInt();
       //QString boite_rattachement = site.at(6);
       QString x = site.at(9);
       QString y = site.at(10);
       mdb->addSite(recno, no, hexacles.at(i),
                    num, suf, voie, code_postal,
                    ville, nbre_foyer,x,y);

   }
    disconnect();

}

void MainFM::disconnect(){
    mdb->disconnect();
    psql->disconnect();
}

void MainFM::on_actionFLR_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Enregistrer le fichier","","Common Separated Value (*.csv)");
    //QFile csv(QDir::tempPath()+"/flr.csv");
    QFile csv(path);
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


    connect();

    psql->connect();
    mdb->connect();



    QStringList hexacles = psql->getAllHexacles();
    int nb_site = hexacles.count();

    progress.setMaximum(nb_site);
    for(int i=0; i<hexacles.length();i++){
        progress.setValue(i);
        if(hexacles.at(i).isEmpty()){
            QMessageBox::warning(this, "Erreur adresse sans hexaclés","Présence d'adresse sans hexaclés, corriger ces erreurs avant import de sites");
            return;
        }
    }

    for(int i=0; i<nb_site;i++){

        progress.setValue(i);
        if(progress.wasCanceled()){
            progress.close();
            break;
        }
        QString hexacle = hexacles.at(i);

        if(hexacle.isEmpty()){
            continue;
            QMessageBox::warning(this, "Export interrompu", "L'export ne peut être finalisé.\nMerci de faire l'audit gege");
            break;
        }
        QStringList values = psql->getSite(hexacle);
        QString num = values.at(0);
        QString suf = values.at(1);
        QString numVoie = suf.isEmpty()?num:num+" "+suf;
        QString adresse=suf.isEmpty()?num:num+" "+suf;
        QString voie = values.at(2);
        adresse =numVoie+" "+voie+" "+values.at(3)+" "+values.at(4);
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

        if(poche.toLower().compare("inexistant")==0){
            parcelle="ADRESSE INEXISTANTE";
        }
        QString pm = psql->getPm(hexacle);
        if(pm.isEmpty()){
            pm="PAS DE PM;0";
        }
        if(hexacle.toLower().contains("crea")){
            hexacle="";
            parcelle="CREATION ADRESSE";
        }
        if(poche.isEmpty()){
            parcelle = "HORS PLAQUE";
            densite="";
            zone="";
            habitat="";
            type_ch="";
            anc_ch="";
            type_tr="";
            anc_tr="";
            gestionnaire="";
            nro="";
            poche="";
            boite="";
            concessionnaire="";
            adduction="";
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

    connect();
    psql->connect();
    mdb->connect();
    QProgressDialog dlg(this);
    dlg.setModal(true);
    dlg.setLabelText("Audit casage...");

    QString path = QFileDialog::getSaveFileName(this, "Enregistrer le fichier","","CSV (*.csv)");

    //QFile csv(QDir::tempPath()+"/audit.csv");
    QFile csv(path);
    csv.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&csv);
    out<<"erreur;hexacle/boîte\n";
    csv.flush();

//    QStringList pmeExcedent = psql->getPME();
//    if(pmeExcedent.length()>0){
//        for(int i=0; i<pmeExcedent.length();i++){
//            out<<"Plus de 100LR affectés au PME;"+pmeExcedent.at(i)+"\n";
//        }
//    }
    QStringList pme=psql->getPME() ;
    int length = pme.length();
    dlg.setMaximum(length);

    for(int i =0; i<length;i++){
        if(!pme.at(i).isEmpty()){
            dlg.setValue(i);
            QStringList infoPme = pme.at(i).split(";");
            if(infoPme.at(1).toInt()>100){
                out<<"Plus de 100 LR affectés au PME;"+infoPme.at(0)+";"+infoPme.at(1)+"\n";
                continue;
            }
            out<<"info;"+infoPme.at(0)+";"+infoPme.at(1)+"\n";
        }

    }

    //retrieve hexacles
    QStringList hexacles = psql->getAllHexacles();
    int nb_sites = hexacles.count();

    dlg.setMaximum(nb_sites);

    //multiple occurance of hexacle
    QStringList multipleHexacle = psql->multipleHexacleOccurence();
    for(int i=0; i<multipleHexacle.length();i++){
        out<<"Hexaclé non unique;"+multipleHexacle.at(i)+"\n";
    }

    //retrieve hexacles
    hexacles = psql->getAllHexaclesInPlaque("plaque");
    nb_sites = hexacles.count();

    dlg.setMaximum(nb_sites);

    QStringList voieChecked;
    QStringList errorNoeudBis;
    for(int i=0; i<nb_sites; i++){
        dlg.setValue(i);

        if(dlg.wasCanceled()){
            break;
        }
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
        QString suf =values.at(1);
        if(suf.length()>1){
            out<<"suf non valide;\""+hexacle+"\"\n";
        }

        //check if voie is valid
        QString voie = values.at(2);
        if(voie.isEmpty() || voie==""){
            if(!voieChecked.contains(voie)){
                out<<"voie non valide;"+voie+";\""+hexacle+"\"\n";
                voieChecked<<voie;
            }

        }
        if(psql->getParcelle(hexacle).isEmpty()){
            out<< "Site mal placé, Numero parcelle incorrecte;\""+hexacle+"\"\n";
        }

        //check if voie is present in Rivoli table

//        bool exists = mdb->isVoieExists(voie);
//        if(!exists){
//            if(!voieChecked.contains(voie)){
//                out<<"voie non présente dans rivoli;"+voie+"\n";
//                voieChecked<<voie;
//            }
//        }



        //check if bal < 0
        if(values.at(5).toInt()<=0){
            out<<"Une adresse existante ne peut avoir 0 LR;\""+hexacle+"\"\n";
        }

        //nommage boite rattachement is correct
        QStringList boites = values.at(6).split(",");
        if(boites.at(0).isEmpty()){
            out<<"boite de rattachement non renseignée;\""+hexacle+"\";"
                 ""+psql->getPoche(hexacle)+"\n";
            boites.clear();
        }
        for(int i=0; i<boites.count();i++){
            QString boite = boites.at(i);
            int poche = psql->getPoche(hexacle).toInt();
            if(!boite.contains("PDB") && !boite.contains("BPI")){
                out<<"nommage netgeo de la boite de rattachement incorrect;"+boite+";\""+hexacle+"\"\n";
                continue;
            }
            if(boite.contains("PDB")){
                if(boite.length()!=19){
                    out<<"nommage netgeo de la boite de rattachement incorrect;"+boite+";\""+hexacle+"\";"
                        ""+QString::number(poche)+"\n";
                    continue;
                }
                if(boite.left(14).compare("PDB_"+pref.getPlaque()+"_")!=0){
                    out<<"nommage netgeo de la boite de rattachement incorrect;"+boite+";\""+hexacle+"\";"
                         ""+QString::number(poche)+"\n";
                    continue;
                }
                if(boite.right(5).left(2).toInt()!=poche){
                    out<<"Incohérence de la poche pour la boite de rattachement et la poche de l'adresse;\""+hexacle+"\"; "
                         ""+QString::number(poche)+"\n";
                    continue;
                }
                QString bis = boite;
                bis.replace("PDB", "PCH");
                bis.insert(14,"0");
                if(!mdb->isNoeudBisExists(bis)){
                    out<<"Boite rattachemente inexistant ou étiquette mal renseignée dans optique;"+hexacle+";"+boite+";"+bis+"\n";
                }

            }
            if(boite.contains("BPI")){
                if(boite.length()!=24){
                    out<<"nommage netgeo de la boite de rattachement incorrect;"+boite+";\""+hexacle+"\";"
                         ""+QString::number(poche)+"\n";
                    continue;
                }
                if(boite.left(14).compare("BPI_SIT_"+pref.getNro()+"_")!=0){
                    out<<"nommage netgeo de la boite de rattachement incorrect;"+boite+";\""+hexacle+"\";"
                         ""+QString::number(poche)+"\n";
                    continue;
                }
                if(boite.right(10).left(4).toInt()!=poche){
                    out<<"Incohérence de la poche pour la boite de rattachement et la poche de l'adresse;\""+hexacle+"\";"
                         ""+QString::number(poche)+"\n";
                    continue;
                }
                QString bis= boite.right(20).left(17);
                if(!mdb->isNoeudBisExists(bis)){
                    out<<"Boite rattachemente inexistant ou étiquette mal renseignée dans optique;"+hexacle+";"+boite+";"+bis+"\n";
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
        if(!concessionnaire.isEmpty()){
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
}

void MainFM::connect(){

    if(psql==NULL){

        psql = new PsqlDatabase();
        psql->setHost(pref.getHost());
        psql->setDatabaseName(pref.getBase());
        psql->setUserName(pref.getUserName());
        psql->setPassword(pref.getPassword());
//        psql->connect();

    }
    if(mdb==NULL){
        mdb=new MdbHandler();
        mdb->setSite(pref.getSite());
        mdb->setOptique(pref.getOptique());
        mdb->setInfra(pref.getInfra());
//        mdb->connect();
    }
}

void MainFM::on_actionAuditCorolle_triggered()
{
    connect();
    mdb->connect();
    QString path = QFileDialog::getSaveFileName(this, "Enregistrer le fichier","","CSV (*.csv)");
    QProgressDialog progress(this);
    progress.setLabelText("Audit Corolle...");
    progress.setModal(true);
    progress.setValue(0);

//    QFile csv(QDir::tempPath()+"/auditCorolle.csv");
    QFile csv(path);
    csv.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&csv);
    out<<"erreur;noeud;poche\n";
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
        int tranche = mdb->getTranche(noeud).toInt();
        //traitement de cable
        if(noeud.contains("L")){
            int poche;
            if(noeud.length()==5){
                poche = noeud.left(2).right(1).toInt();
            }
            if(noeud.length()==6){
                poche = noeud.left(3).right(2).toInt();
            }
            if(poche!=tranche){
                out<<"Incohérence du noeud et de la poche;"+noeud+";"+QString::number(poche)+";"+QString::number(tranche)+"\n";
            }
            if(!mdb->cableHasCorrectExtremite(noeud)){
                out<<"Extrémité du noeud incorrecte;"+noeud+"\n";
            }
            if(!mdb->cableHasOrigine(noeud)){
                out<<"Origine du noeud incorrecte;"+noeud+"\n";
            }
            QString noeudBis = mdb->getNoeudBis(noeud);
            if(noeudBis.right(16).left(11).compare("_"+pref.getPlaque()+"_")!=0){
                out<<"cable mal nommé; "+noeud+";"+noeudBis+";"+noeudBis.right(16)+"\n";
            }
            if(noeudBis.length()==6){
                if(noeudBis.right(5).left(2).toInt()!=poche){
                    out<<"Incohérence entre la poche du noeud et le nom du cable;"+noeud+";"+noeudBis+"\n";
                }
            }

        }
        else{
            int poche=0;
            if(noeud.length()==5){
                poche = noeud.left(2).toInt();
            }
            if(noeud.length()==4){
                poche = noeud.left(1).toInt();
            }
            if(poche!=mdb->getTranche(noeud).toInt()){
                out<<"Incohérence du noeud et de la poche;"+noeud+";"+QString::number(tranche)+"\n";
            }
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
    QMessageBox::information(this, "Fin audit", "Audit corolle terminé");

}

void MainFM::on_actionMAJ_Base_CTR_triggered()
{
   sqlDlg = new ExecSqlDialog(this);

    sqlDlg->show();
}

void MainFM::on_actionLTA_triggered()
{
    connect();
    psql->connect();
    mdb->connect();

    QProgressDialog progress(this);
    progress.setLabelText("Association LTA...");
    progress.setModal(true);
    progress.setValue(0);

    QStringList sitesNo = mdb->getAllSite();
    progress.setMaximum(sitesNo.length());

    for(int i =0; i<sitesNo.length();i++){

        progress.setValue(i);
        if(progress.wasCanceled()){
            progress.close();
            break;
        }


        QStringList site = sitesNo.at(i).split(";");
        QString recno = mdb->nextRecno("lindsite");
        QString no = site.at(0);
        QString hexacode = site.at(1);
        QStringList  currentSite = psql->getSite(hexacode);
        if(currentSite.isEmpty()){
            qDebug()<<hexacode;
            continue;
        }
        QStringList boites = currentSite.at(6).split(";");

        QString boite = boites.at(0).trimmed();
        QString noeud="";
        if(boite.contains("BPI")){
            noeud=mdb->getCableAmont(QString::number(boite.right(10).left(7).toInt()));
        }
        if(boite.contains("PDB")){
            noeud=QString::number(boite.right(5).toInt());
            QStringList noeuds = mdb->getCableAval(noeud);
            //test if noeud is not empty

            if(noeuds.length()!=0){
                noeud = noeuds.at(0);
            }
            else{
                noeud="";
            }
        }
        //insertion du site
        mdb->setLindSite(recno, no,numSect, noeud);
    }
    disconnect();

}

void MainFM::on_actionA_propos_triggered()
{
    QMessageBox::information(this,"A propos","FiberManager-e\nApplication d'aide au contrôle de la base Corolle/Netgeo.\n\nCopyright Njeneza François.");
}

void MainFM::on_actionGC_2_triggered()
{
    doegc=new DOEGCdlg();
    doegc->setModal(true);
    doegc->show();
}
