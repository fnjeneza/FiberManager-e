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


bool MdbHandler::connect(){
    int connection=0;
    if(!optique.isEmpty()){
        optique_db = QSqlDatabase::addDatabase("QODBC","optique" );
        optique_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+optique);
        if(!optique_db.isOpen()){
            if(!optique_db.open()){
                qDebug()<<optique_db.lastError().text();
                return false;
            }
            connection++;
        }
        else{
            connection++;
        }

    }
    else{
        qDebug()<<"Chemin vers l'optique non défini";
    }
    if(!site.isEmpty()){
        site_db = QSqlDatabase::addDatabase("QODBC", "site");
        site_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+site);
        if(!site_db.isOpen()){
            if(!site_db.open()){
                qDebug()<<site_db.lastError().text();
                return false;
            }
            connection++;
        }
        else{
            connection++;
        }

    }
    else{
        qDebug()<<"Chemin vers Site non défini";
    }
    if(!infra.isEmpty()){
        infra_db = QSqlDatabase::addDatabase("QODBC", "infra");
        infra_db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+infra);
        if(!infra_db.isOpen()){
            if(!infra_db.open()){
                qDebug()<<infra_db.lastError().text();
                return false;
            }
            connection++;
        }
        else{
            connection++;
        }

    }
    else{
        qDebug()<<"Chemin vers Infra non défini";
    }

    if(connection==3){
        return true;
    }
    return false;

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

bool MdbHandler::isRivoliEmpty(QString noeud){
    QString query = "select RIVOLI from noeuds "
                    "where NOEUD='"+noeud+"' AND DELETED<>'*'";
    QSqlQuery q = infra_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    if(q.next()){
        return q.value("RIVOLI").toString().trimmed().isEmpty();
    }
    return false;
}

bool MdbHandler::isEtiquetteEmpty(QString noeud){
    QString query = "select NOEUD_BIS, REF_MAT from noeuds "
                    "where NOEUD='"+noeud+"' AND DELETED<>'*' AND REF_MAT<>'175'";
    QSqlQuery q = infra_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    if(q.next()){
        int refMat = q.value("REF_MAT").toInt();
        if(refMat!=175){
            return q.value("NOEUD_BIS").toString().trimmed().isEmpty();
        }
    }
    return false;
}

QString MdbHandler::multipleFTEtiquetteOccurence(QString noeud){
    QString query = "select NOEUD_BIS from noeuds "
                    "where NOEUD='"+noeud+"'";
    QSqlQuery q = infra_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString etiquette;
    etiquette.clear();
    if(q.next()){
        etiquette=q.value("NOEUD_BIS").toString().trimmed();
    }

    query = "select NOEUD_BIS,INSEE_COM, count(NOEUD_BIS) as nb from noeuds "
                    "where trim(NOEUD_BIS)='"+etiquette+"' group by NOEUD_BIS, INSEE_COM";
    q = infra_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString value;
    value.clear();
    if(q.next()){
        QString noeud = q.value("NOEUD_BIS").toString().trimmed();
        int nb = q.value("nb").toInt();
        if(nb>1){
            value=noeud;
        }
    }
    return value;
}

QStringList MdbHandler::getUsedInfraNoeud(){
    QString query = "select distinct CHAMBRE "
                    "from affectat "
                    "where DELETED<>'*' and CHAMBRE LIKE 'C%'";
    QSqlQuery q = infra_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList values;
    values.clear();
    while(q.next()){
        values<<q.value("CHAMBRE").toString().trimmed();
    }
    return values;
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

QString MdbHandler::getTranche(QString noeud){
    QString query = "select TRANCHE from noeuds "
                    " where DELETED<>'*' and NOEUD='"+noeud+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString value;
    while(q.next()){
        value = q.value("TRANCHE").toString().trimmed();
    }
    return value;
}

bool MdbHandler::cableHasCorrectExtremite(QString noeud){
    QString query = "select COFFRET from noeuds "
                    " where DELETED<>'*' and NOEUD='"+noeud+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString value;
    while(q.next()){
        value = q.value("COFFRET").toString().trimmed();
    }
    if(value!=noeud.right(noeud.length()-1)){
        return false;
    }
    return true;
}

QString MdbHandler::getNoeudBis(QString noeud){
    QString query = "select NOEUD_BIS from noeuds "
                    " where DELETED<>'*' and NOEUD='"+noeud+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString value;
    while(q.next()){
        value = q.value("NOEUD_BIS").toString().trimmed();
    }
    return value;
}

//QString MdbHandler::getComadre(QString noeud){
//    QString query = "select NOEUD_BIS from noeuds "
//                    " where DELETED<>'*' and NOEUD='"+noeud+"'";
//    QSqlQuery q = optique_db.exec(query);
//    if(!q.isActive()){
//        qDebug()<<q.lastError().text();
//    }
//    QString value;
//    while(q.next()){
//        value = q.value("NOEUD_BIS").toString().trimmed();
//    }
//    return value;
//}

bool MdbHandler::isNoeudBisExists(QString noeudbis){
    QString query = "select NOEUD from noeuds "
                    "where DELETED<>'*' AND "
                    "NOEUD_BIS='"+noeudbis+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    return q.next();
}

bool MdbHandler::cableHasOrigine(QString noeud){
    QString query = "select N_AMONT from noeuds "
                    " where DELETED<>'*' and NOEUD='"+noeud+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString value;
    while(q.next()){
        value = q.value("N_AMONT").toString().trimmed();
    }
    if(value=="-1"){
        return false;
    }
    return true;
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

QString MdbHandler::getComadre(QString noeud){
    QString query = "select COMADRE from pb "
                    "where NOEUD='"+noeud+"' and DELETED<>'*'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString value;
    value.clear();
    if(q.next()){
        value=q.value("COMADRE").toString().trimmed();
    }
    return value;
}

QStringList MdbHandler::getAssociatedChambre(QString noeud){
    QString query = "select CHAMBRE from affectat "
                    "where trim(NOEUD)='"+noeud+"' AND DELETED<>'*'";
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

QStringList MdbHandler::getDeployedCable(){
    QString query = "select NOEUD, TRANCHE, LONG_CABLE, TYPE_CABLE, NOEUD_BIS from noeuds "
                    "where RECETTE_CA='T' AND DELETED<>'*'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList values;
    values.clear();
    while(q.next()){
        values<< q.value("NOEUD").toString().trimmed()+";"
                       ""+q.value("TRANCHE").toString().trimmed()+";"
                       ""+q.value("LONG_CABLE").toString().trimmed()+";"
                        ""+q.value("NOEUD_BIS").toString().trimmed()+";"
                       ""+q.value("TYPE_CABLE").toString().trimmed();

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

void MdbHandler::setLindSite(QString recno, QString no, QString numSect, QString noeud){
    QString query = "insert into lindsite (RECNO, NO_SITE,NUM_SECT, NOEUD) values("
                    "'"+recno+"','"+no+"','"+numSect+"','"+noeud+"')";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
}

QStringList MdbHandler::getAllSite(){
    QString query = "select \"NO\", HEXACODE  from sites";
    QSqlQuery q = optique_db.exec(query);
    QStringList nos;
    while(q.next()){
        nos<<q.value("NO").toString().trimmed()+";"+q.value("HEXACODE").toString().trimmed();
    }
    return nos;
}

QString MdbHandler::getCableAmont(QString noeud){
    QString query = "select NOEUD from noeuds where COFFRET='"+noeud+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString n="";
    while(q.next()){
        n=q.value("NOEUD").toString().trimmed();
    }
    return n;
}


QStringList MdbHandler::getCableAval(QString noeud){
    QString query = "select NOEUD from noeuds where N_AMONT='"+noeud+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList noeuds;
    noeuds.clear();
    while(q.next()){
        noeuds<<q.value("NOEUD").toString().trimmed();
    }
    return noeuds;
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

void MdbHandler::disconnect(){
    if(optique_db.isOpen()){
        optique_db.close();
    }
    if(infra_db.isOpen()){
        infra_db.close();
    }
    if(site_db.isOpen()){
        site_db.close();
    }
}

void MdbHandler::addSite(QString recno, QString no, QString hexacle, QString num, QString suf, QString voie, QString code_postal, QString commune, int bal, QString x, QString y){
    QString numvoie=suf.isEmpty()?num:num+" "+suf;
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
//controsingle name
QString MdbHandler::controlNommage(QString plaque, QString noeud){
    QString query = "select NOEUD, TRANCHE, NOEUD_BIS, COMADRE "
                    "from NOEUDS"
                    "where DELETED<>'*'"
                    "and NOEUD='"+noeud+"'";
    QSqlQuery q = optique_db.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }

    while(q.next()){

    }
}
