#include "fmprocedure.h"

FMProcedure::FMProcedure()
{

}

FMProcedure::~FMProcedure()
{
}

void FMProcedure::setOptiquePath(QString path){
    optiquePath=path;
}

void FMProcedure::showMessage(QString message){
    qDebug()<<message;
}

void FMProcedure::generateDoeRepository(QString path, QString nro, QString poche, QString plaque, QString ext){
    QDate current=QDate::currentDate();
    QString date=QString::number(current.year()).right(2)+QString::number(current.month())+QString::number(current.day());

    QDir root(path);
    if(!root.exists()){
       showMessage("Repertoire non trouvé");
        return;
    }

    QString rep_501;
    if(ext.isEmpty()){
        rep_501="501_DOE_CTR"+poche+"_"+nro+"_"+ext+"_"+date+"_INDA";
    }
    else{
        rep_501="501_DOE_CTR"+poche+"_"+nro+"_"+date+"_INDA";
    }
    QStringList arborescence;
    arborescence.append(rep_501+"/415_FAT_TVX_"+nro);
    arborescence.append(rep_501+"/515_BASE_CTR"+poche+"_DOE_"+nro+"_INDA");
    arborescence.append(rep_501+"/521_PHOTOS_CTR"+poche+"_TVX_"+nro+"_INDA");
    arborescence.append(rep_501+"/530_INF_GC_PLN_TVX_"+nro);
    arborescence.append(rep_501+"/550_DOSS_INFRA_TVX_"+nro+"_INDA");
    arborescence.append(rep_501+"/580_DM_TVX_"+nro+"_INDA/581_DM_TVX_CTR"+poche+"_"+nro+"_INDA/CTR_"+plaque+"_"+poche+"_INDA");
    arborescence.append(rep_501+"/591_JUSTIFICATION_DOE_CTR"+poche+"_"+nro+"_"+date+"_INDA");

    for(int i=0; i<arborescence.size(); i++){
        root.mkpath(arborescence.at(i));
    }
}

void FMProcedure::controlDoeRepository(QString path){

}
void FMProcedure::connectToOptiqueMdb(){
    optiqueDb = QSqlDatabase::addDatabase("QODBC");
    if(optiquePath.isEmpty()){
        showMessage("Optique path is empty");
        return;
    }
    optiqueDb.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+optiquePath);
    if(!optiqueDb.open()){
            qDebug()<<optiqueDb.lastError().text();
    }

}


void FMProcedure::extractSiteFromOptique(){


}

void FMProcedure::extractNoeudsFromOptique(){

    if(optiqueDb.isOpen()){
        showMessage("Optique non connecté");
    }
    QSqlQuery query = optiqueDb.exec("select * from noeuds");
    while(query.next()){
        QString noeud = query.value(2).toString();
        qDebug()<<noeud;
    }
}

void FMProcedure::closeConnectionToOptiqueMdb(){
    if(optiqueDb.isOpen()){
        optiqueDb.close();
    }
}




void FMProcedure::exportFlr(){
    DbHandler db=DbHandler();
    QSqlDatabase flr = db.connectToPostgresServer("flr");
    QSqlQuery query = flr.exec("select * from flr limit 1");
    if(!query.isActive()){
        qDebug()<< query.lastError().text();
        return;
    }
    QSqlRecord record = query.record();
    for(int i=0; i<record.count(); i++){
        qDebug()<<record.fieldName(i);
    }



    while(query.next()){
        QString hexacle = query.value("code").toString();
        int bal = query.value("num").toInt();
        QString num_parc = query.value("num_parcelle").toString();
        QString type_chaussee = query.value("type_chaussee").toString();
        int anc_chaussee = query.value("anc_chaussee").toInt();
        QString type_trottoir = query.value("type_trottoir").toString();
        int anc_trottoir = query.value("anc_trottoir").toInt();
        QString gestionnaire = query.value("gestionnaire").toString();
        int poche = query.value("poche").toInt();
        QString boite_rattachement = query.value("pme").toString();
        QString adduction = query.value("adduction").toString();
        QString concessionnaire = query.value("concessionnaire").toString();
        QString reference = query.value("reference").toString();
        QString lr_pm = query.value("logements_pm").toString();
        QString commentaire = query.value("commentaire").toString();
        QString adresse = Address::getCompleteAddress(query.value("num").toInt(),query.value("suf").toString(), query.value("voie").toString());


    }
    flr.close();
}
