#include "mdbhandler.h"

MdbHandler::MdbHandler()
{

}

MdbHandler::~MdbHandler()
{
    infra_db.close();
    optique_db.close();
    site_db.close();
}


//void MdbHandler::connect(QString ConnectionName){
//    optique_db = QSqlDatabase::addDatabase("QODBC", ConnectionName);
//    optique_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+optique);
//    if(!optique_db.open()){
//        qDebug()<<optique_db.lastError().text();
//    }
//    optique_db = QSqlDatabase::addDatabase("QODBC", ConnectionName);
//    optique_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+site);
//    if(!optique_db.open()){
//        qDebug()<<optique_db.lastError().text();
//    }

//}

void MdbHandler::connect(){

    if(!optique.isEmpty()){
        optique_db = QSqlDatabase::addDatabase("QODBC","optique" );
        optique_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+optique);
        if(!optique_db.open()){
            qDebug()<<optique_db.lastError().text();
        }
    }
    else{
        qDebug()<<"Chemin vers l'optique non défini";
    }
    if(!site.isEmpty()){
        site_db = QSqlDatabase::addDatabase("QODBC", "site");
        site_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+site);
        if(!site_db.open()){
            qDebug()<<site_db.lastError().text();
        }
    }
    else{
        qDebug()<<"Chemin vers Site non défini";
    }
    if(!infra.isEmpty()){
        infra_db = QSqlDatabase::addDatabase("QODBC", "infra");
        infra_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+infra);
        if(!infra_db.open()){
            qDebug()<<infra_db.lastError().text();
        }
    }
    else{
        qDebug()<<"Chemin vers Infra non défini";
    }

}
QString MdbHandler::getInfraNoeudAdresse(QString noeud){
    QString query = "select NUM_VOIE, RIVOLI from noeuds "
                    "where NOEUD='"+noeud+"' AND DELETED<>'*'";
    QSqlQuery q = infra_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString adresse;
    if(q.next()){
        adresse = q.value("NUM_VOIE").toString().trimmed()+" "+
                q.value("RIVOLI").toString().trimmed();
    }
    return adresse;
}

QString MdbHandler::getOptiqueNoeudAdresse(QString noeud){
    QString query = "select NUM_VOIE, RIVOLI from noeuds "
                    "where NOEUD='"+noeud+"' AND DELETED<>'*'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString adresse;
    if(q.next()){
        adresse = q.value("NUM_VOIE").toString().trimmed()+" "+
                q.value("RIVOLI").toString().trimmed();
    }
    return adresse;
}

QStringList MdbHandler::getAllOptiqueNoeud(){
    QString query = "select NOEUD from noeuds "
                    " where DELETED<>'*'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList values;
    while(q.next()){
        values<<q.value("NOEUD").toString().trimmed();
    }
    return values;
}

QStringList MdbHandler::getAssociatedChambre(QString noeud){
    QString query = "select CHAMBRE from affectat "
                    "where NOEUD='"+noeud+"' AND DELETED<>'*'";
    QSqlQuery q = infra_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList values;
    values.clear();
    while(q.next()){
        values<<q.value("CHAMBRE").toString();
    }
    return values;
}

void MdbHandler::setOptique(QString optique){
    this->optique=optique;
}

void MdbHandler::setInfra(QString infra){
    this->infra = infra;
}

void MdbHandler::setSite(QString site){
    this->site = site;
}

bool MdbHandler::siteExists(QString hexacle, QString num, QString suf, QString voie){
    QString num_voie = suf.isEmpty()?num:num+suf;
    QString query = "SELECT HEXACODE, NUMVOIE, ADRESSE "
                    "FROM sites "
                    "WHERE HEXACODE= '"+hexacle+"' OR "
                          "ADRESSE='"+num_voie+" "+voie+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    return q.next();
}

QString MdbHandler::lastRecno(QString tableName){
    QString query = "SELECT max(RECNO) FROM "+tableName;
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<< q.lastError().text();
        return NULL;
    }
    q.next();
    return q.value(0).toString();
}

QString MdbHandler::lastSiteNumber(){
    QString query = "SELECT  * FROM sites";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<< q.lastError().text();
        return NULL;
    }
    int max=0;
    bool ok;
    while(q.next()){
        int current = q.value("NO").toString().right(5).toInt(&ok, 10);
        max=current>max?current:max;
    }
    return QString::number(max);
}

QString MdbHandler::nextRecno(QString tableName){
    QString recno=lastRecno(tableName);
    if(recno.isEmpty()){
        recno="1";
    }
    bool ok;
    return QString("00000000"+QString::number(recno.toInt(&ok, 16)+1,16)).right(8).toUpper();
}

QString MdbHandler::nextSiteNumber(){
    return "S"+QString("00000"+QString::number(lastSiteNumber().toInt()+1,10)).right(5);
}

QStringList MdbHandler::tables(int schema){
    switch (schema) {
    case 1:
        return optique_db.tables(QSql::Tables);
        break;
    default:
        return QStringList();
        break;
    }

}

QString MdbHandler::structure(int schema, QString tableName){
    QSqlDatabase db;
    QString prefix;
    switch (schema) {
    case 1:
        db=optique_db;
        prefix = "optique";
        break;
    default:
        break;
    }
    QSqlRecord record = db.record(tableName);
    int length = record.count();
    QString description="CREATE TABLE "+prefix+"."+tableName+"(\n";
    for(int i = 0; i<length;i++){
        QString name = record.field(i).name().toLower();
        if(i<length-1){
            //en supposant que toute valeur est un text
            description+=name+"\t"+"text,\n";
        }
        else{
            description+=name+"\t"+"text\n";
        }
    }
    description+=")";
    return description;
}

bool MdbHandler::needUpdate(QString hexacle, QString num, QString suf, QString voie, int bal, QString x, QString y){
    QString query="SELECT HEXACODE, ADRESSE, NBPRISE, COORD_X, COORD_Y FROM sites WHERE HEXACODE='"+hexacle+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    if(q.next()){
        QString numvoie = suf.isEmpty()?num:num+" "+suf;
        QString adresse = numvoie+" "+voie;
        if(q.value("ADRESSE").toString().trimmed().compare(adresse)!=0){
            return true;
        }
        if(q.value("NBPRISE").toInt()!=bal){
            return true;
        }
        if(q.value("COORD_X").toString().trimmed().compare(x)!=0 ||
               q.value("COORD_Y").toString().trimmed().compare(y)!=0 ){
            return true;
        }
    }
    return false;
}


void MdbHandler::addSite(QString recno, QString no, QString hexacle, QString num, QString suf, QString voie, QString code_postal, QString commune, int bal, QString x, QString y){
    QString numvoie=suf.isEmpty()?num:num+" "+voie;
    QString adresse = numvoie+" "+voie;
    QString query="INSERT INTO sites (RECNO, NO, ID_TYPSIT, ID_TYPFCT, ID_ETAT, NUMVOIE, ADRESSE, CODE_POST,"
                    "COMMUNE, HEXACODE, NBPRISE, ID_MODRAC, PROPRIETE, ID_PROBADR, COORD_X, COORD_Y)"
                    "VALUES ('"+recno+"','"+no+"','0','0','0','"+numvoie+"','"+adresse+"','"+code_postal+"','"+commune+"','"+hexacle+"','"+QString::number(bal)+"','0','PRIVE','0','"+x+"','"+y+"')";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
}

void MdbHandler::updateSite(QString hexacle, QString num, QString suf, QString voie, QString code_postal, QString commune, int bal, QString x, QString y){
    QString numvoie=suf.isEmpty()?num:num+" "+voie;
    QString adresse = numvoie+" "+voie;
    QString query="UPDATE sites SET "
                  "NUMVOIE='"+numvoie+"', "
                  "ADRESSE='"+adresse+"', "
                  "CODE_POST='"+code_postal+"', "
                  "COMMUNE='"+commune+"', "
                  "NBPRISE='"+QString::number(bal)+"', "
                  "COORD_X='"+x+"', "
                  "COORD_Y='"+y+"'  WHERE HEXACODE='"+hexacle+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive())
        qDebug()<<q.lastError().text();
    qDebug()<<query;
}

QStringList MdbHandler::retrieveRivoliRecno(){
    QString query = "select RECNO from rivoli";
    QSqlQuery q = site_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList values;
    while(q.next()){
        values<<q.value("RECNO").toString();
    }
    return values;
}
//QString MdbHandler::getVoie(QString rivoli){
//    QString query = "select TYPE_VOIE, ARTICLE, DESIGNATIO from rivoli where RIVOLI='"+rivoli+"'";
//    QSqlQuery q = site_db.exec(query);
//    if(!q.isActive()){
//        qDebug()<<q.lastError().text();
//    }
//    QString voie;
//    if(q.next()){
//        QString type_voie=q.value("TYPE_VOIE").toString().trimmed();
//        QString article = q.value("ARTICLE").toString().trimmed();
//        QString designatio = q.value("DESIGNATIO").toString().trimmed();
//        voie = article.isEmpty()?type_voie+" "+designatio:type_voie+" "+article+" "+designatio;
//        qDebug()<<voie;
//    }

//    return voie;
//}

//void MdbHandler::loadRivoli(){
//    QString query = "SELECT RIVOLI, TYPE_VOIE, ARTICLE, DESIGNATIO FROM rivoli";
//    QSqlQuery q = site_db.exec(query);
//    if(!q.isActive()){
//        qDebug()<<q.lastError().text();
//    }
//    if(q.next()){
//        QString riv = q.value("RIVOLI").toString();
//        QString voie = getVoie(riv);
//        rivoli.insert(riv, voie);
//    }
//}


bool MdbHandler::isZoneAvant(QString num, QString suf, QString voie){
    QString query ;
    QSqlQuery q ;
    loadRivoli();
    QString rivoli_ = rivoli.key(voie);

    QString num_voie=QString(num+suf).trimmed();
    query = "SELECT NUM_VOIE, RIVOLI from noeuds "
            "where NOEUD_BIS LIKE 'SIT%' "
            "AND RIVOLI='"+rivoli_+"' "
            "AND NUM_VOIE='"+num_voie+"'";
    q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    return q.next();
}

bool MdbHandler::isVoieExists(QString voie){
    return (rivoli.value(voie)).isEmpty()?false:true;
}

//QString MdbHandler::getRivoli(QString voie){
//    if(!isRivoliLoaded){
//        return rivoli.key(voie);
//    }
//}

void MdbHandler::loadRivoli(){
    if(!isRivoliLoaded){
        qDebug()<<"Loading rivoli";
        //load rivoli
        QString query = "SELECT RIVOLI, TYPE_VOIE, ARTICLE, DESIGNATIO FROM rivoli";
        QSqlQuery q = site_db.exec(query);
        if(!q.isActive()){
            qDebug()<<q.lastError().text();
        }
        while(q.next()){
            QString riv = q.value("RIVOLI").toString().trimmed();
            QString type_voie=q.value("TYPE_VOIE").toString().trimmed();
            QString article = q.value("ARTICLE").toString().trimmed();
            QString designatio = q.value("DESIGNATIO").toString().trimmed();
            QString voie = article.isEmpty()?type_voie+" "+designatio:type_voie+" "+article+" "+designatio;
            rivoli.insert(riv, voie);
        }
        isRivoliLoaded=true;
        qDebug()<<"Rivoli loaded";
    }
}
