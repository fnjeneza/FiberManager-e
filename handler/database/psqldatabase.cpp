#include "psqldatabase.h"

PsqlDatabase::PsqlDatabase()
{

}

PsqlDatabase::~PsqlDatabase()
{
    if(psql.open())
        psql.close();
}

void PsqlDatabase::connect(){
    psql = QSqlDatabase::addDatabase("QPSQL");
    psql.setHostName(host);
    //qDebug()<<"port"<<port;
    psql.setDatabaseName(databaseName);
    psql.setUserName(userName);
    psql.setPassword(password);
    if(!psql.isOpen()){
        if(!psql.open()){
            qDebug()<< psql.lastError().text();
        }
    }

}

QStringList PsqlDatabase::getPME(){
    QString query = "select boite_rattachement, sum(nbre_foyer) as total_lr "
                    "from casage where boite_rattachement LIKE 'PDB%' "
                    "group by boite_rattachement";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList values;
    values.clear();
    while(q.next()){
        int total = q.value("total_lr").toInt();
        values<<q.value("boite_rattachement").toString()+";"+QString::number(total);
    }
    return values;
}

void PsqlDatabase::disconnect(){
    if(psql.isOpen()){
        psql.close();
    }
}

QStringList PsqlDatabase::columns(QString tableName){
    QSqlRecord record = psql.record(tableName);
    QStringList columns;
    for(int i=0;i<record.count();i++){
        columns<<record.fieldName(i);
    }
    return columns;
}

/**
 * @brief PsqlDatabase::retrieveAdresses
 * Retrieve adresses separeted by semi-colon
 * hexacle, num, suf, voie
 * @return
 */
QStringList PsqlDatabase::retrieveAdresses(){
    QString query  = "select code, num, suf, voie from casage";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList adresses;
    while(q.next()){
        adresses<<q.value("code").toString()+";"+q.value("num").toString()+";"+q.value("suf").toString()+";"+q.value("voie").toString();
    }
    return adresses;
}


QStringList PsqlDatabase::lr(QString hexacle, QString num, QString suf, QString voie){
    QString query = "select * from flr "
                    "where code='"+hexacle+"' "
                          "AND num='"+num+"' "
                          "AND suf='"+suf+"' "
                          "AND voie='"+voie+"'";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList value;
    if(q.next()){
        QString hexacle = q.value("code").toString();
        QString bal = q.value("num").toString();
        QString num_parc = q.value("num_parcelle").toString();
        QString type_chaussee = q.value("type_chaussee").toString();
        QString anc_chaussee = q.value("anc_chaussee").toString();
        QString type_trottoir = q.value("type_trottoir").toString();
        QString anc_trottoir = q.value("anc_trottoir").toString();
        QString gestionnaire = q.value("gestionnaire").toString();
        QString poche = q.value("poche").toString();
        QString boite_rattachement = q.value("boite_rattachement").toString().replace(";",",");
        QString adduction = q.value("adduction").toString();
        QString concessionnaire = q.value("concessionnaire").toString();
        QString reference = q.value("reference").toString().replace(";",",");
        QString lr_pm = q.value("logements_pm").toString();
        QString commentaire = q.value("commentaire").toString().replace(";",",");

        //adresse
//        QString num = q.value("num").toString();
//        QString suf = q.value("suf").toString();
        QString numvoie = suf.isEmpty()?num:num+" "+suf;
//        QString voie = q.value("voie").toString();
        QString adresse = numvoie+" "+voie+" "+q.value("code_postal").toString()+" "+q.value("ville").toString();

        QString zone="Zone Arriere";
    //        bool isZone = isFrontZone(q.value("num").toString().trimmed(),
    //                                    q.value("suf").toString().trimmed(),
    //                                    q.value("voie").toString().trimmed());
    //        if(isZone){
    //            zone="Zone Avant";
    //        }
        QString nro=this->nro;
        QString phd="PHD";
        QString type_habitat=bal.toInt()>1?"COLL":"INDIV";

        if(reference.isEmpty()){
            reference="PAS DE PM";
        }
        if(adduction.isEmpty()){
            adduction="GC";
            concessionnaire="FT";
        }
        if(concessionnaire.isEmpty()){
            concessionnaire="FT";
        }
        value<<hexacle<<adresse<<bal<<num_parc<<type_chaussee<<anc_chaussee<<type_trottoir
            <<anc_trottoir<<gestionnaire<<poche<<boite_rattachement<<adduction<<concessionnaire
           <<reference<<lr_pm<<commentaire+"\n";
    }
    return value;

}

QString PsqlDatabase::flr(){
    QString query = "select * from flr";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<< q.lastError().text();
    }
    QString out;
    while(q.next()){
        QString hexacle = q.value("code").toString();
        QString bal = q.value("num").toString();
        QString num_parc = q.value("num_parcelle").toString();
        QString type_chaussee = q.value("type_chaussee").toString();
        QString anc_chaussee = q.value("anc_chaussee").toString();
        QString type_trottoir = q.value("type_trottoir").toString();
        QString anc_trottoir = q.value("anc_trottoir").toString();
        QString gestionnaire = q.value("gestionnaire").toString();
        QString poche = q.value("poche").toString();
        QString boite_rattachement = q.value("boite_rattachement").toString().replace(";",",");
        QString adduction = q.value("adduction").toString();
        QString concessionnaire = q.value("concessionnaire").toString();
        QString reference = q.value("reference").toString().replace(";",",");
        QString lr_pm = q.value("logements_pm").toString();
        QString commentaire = q.value("commentaire").toString().replace(";",",");

        //adresse
        QString num = q.value("num").toString();
        QString suf = q.value("suf").toString();
        QString numvoie = suf.isEmpty()?num:num+" "+suf;
        QString voie = q.value("voie").toString();
        QString adresse = numvoie+" "+voie+" "+q.value("code_postal").toString()+" "+q.value("ville").toString();

        QString zone="Zone Arriere";
//        bool isZone = isFrontZone(q.value("num").toString().trimmed(),
//                                    q.value("suf").toString().trimmed(),
//                                    q.value("voie").toString().trimmed());
//        if(isZone){
//            zone="Zone Avant";
//        }
        QString nro=this->nro;
        QString phd="PHD";
        QString type_habitat=bal.toInt()>1?"COLL":"INDIV";

        if(reference.isEmpty()){
            reference="PAS DE PM";
        }
        if(adduction.isEmpty()){
            adduction="GC";
            concessionnaire="FT";
        }
        if(concessionnaire.isEmpty()){
            concessionnaire="FT";
        }

        out+=hexacle+";"+adresse +";"+bal+";"+phd+";"+num_parc+";"+zone+";"+type_habitat +";"+type_chaussee+";"
          ""+anc_chaussee +";"+type_trottoir +";"+anc_trottoir +";"+gestionnaire +";"+nro+";"+ poche+";"+boite_rattachement+";"+ adduction +";"
          ""+concessionnaire+";"+ reference+";"+ lr_pm+";"+ commentaire+"\n";
    }
    return out;
}

QString PsqlDatabase::getHost() const
{
    return host;
}

void PsqlDatabase::setHost(const QString &value)
{
    host = value;
}

QString PsqlDatabase::getDatabaseName() const
{
    return databaseName;
}

void PsqlDatabase::setDatabaseName(const QString &value)
{
    databaseName = value;
}
QString PsqlDatabase::getUserName() const
{
    return userName;
}

void PsqlDatabase::setUserName(const QString &value)
{
    userName = value;
}
QString PsqlDatabase::getPassword() const
{
    return password;
}

void PsqlDatabase::setPassword(const QString &value)
{
    password = value;
}
int PsqlDatabase::getPort() const
{
    return port;
}

void PsqlDatabase::setPort(int value)
{
    port = value;
}
QString PsqlDatabase::getNro() const
{
    return nro;
}

void PsqlDatabase::setNro(const QString &value)
{
    nro = value;
}

//QStringList PsqlDatabase::adressesWithoutHexacode(){
//    QString query = "SELECT num, suf, voie FROM casage where code is null";
//    QSqlQuery q = psql.exec(query);
//    QStringList values;

//    if(!q.isActive()){
//        qDebug()<<q.lastError().text();
//    }
//    while(q.next()){
//        values<<"Hexacle non renseigné;"<<q.value("num").toString()+";"+q.value("suf").toString()+";"+q.value("voie").toString();
//    }
//    return values;

//}



QStringList PsqlDatabase::getAllHexacles(){
    QString query = "select code from casage";
    QSqlQuery q = psql.exec(query);
    QStringList values;
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    while(q.next()){
        values<<q.value(0).toString();
    }
    return values;
}

QStringList PsqlDatabase::getAllHexaclesInPlaque(QString shapePlaque){
    QString query="select code from casage, \""+shapePlaque+"\" where st_intersects(casage.the_geom, \""+shapePlaque+"\".the_geom)";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList hexacles;
    while(q.next()){
        hexacles<<q.value("code").toString();
    }
    return hexacles;
}

QStringList PsqlDatabase::getSite(QString hexacle){
    if(hexacle=="1321522NRL"){
        qDebug()<<"";
    }
    QString query ="select num, suf, voie, code_postal, ville, nbre_foyer, boite_rattachement, adduction, concessionnaire, ST_X(the_geom) as x, ST_Y(the_geom) as y, commentaire "
                   "from casage "
                   "where code='"+hexacle+"'";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList values;
    if(q.next()){
        QString x = q.value("x").toString();
        QString y = q.value("y").toString();
        QString commentaire = q.value("commentaire").toString().replace(";",",");
        int index = x.indexOf(".");
        if(index>0){
            x = x.left(index+4);
        }
        index = y.indexOf(".");
        if(index>0){
            y = y.left(index+4);
        }

        values<<q.value("num").toString().trimmed()
             <<q.value("suf").toString().toUpper().trimmed()
             <<q.value("voie").toString().toUpper().trimmed()
             <<q.value("code_postal").toString().trimmed()
             <<q.value("ville").toString().toUpper().trimmed()
             <<q.value("nbre_foyer").toString().trimmed()
             <<q.value("boite_rattachement").toString().replace(";",",").trimmed()
            <<q.value("adduction").toString().trimmed()
            <<q.value("concessionnaire").toString().trimmed()
          <<x
         <<y
        <<commentaire;
    }
    return values;
}

QString PsqlDatabase::getPm(QString hexacle){
    QString query = "select reference, nombre_de_logements from pm where hexacle='"+hexacle+"'";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QString ref;
    int lr;
    while(q.next()){
        if(ref.isEmpty()){
            ref=q.value("reference").toString();
            lr = q.value("nombre_de_logements").toInt();
        }else{
            ref=ref+", "+q.value("reference").toString();
            lr = lr+q.value("nombre_de_logements").toInt();
        }
    }
    if(!ref.isEmpty()){
        return ref+";"+QString::number(lr);
    }
    return QString();
}

QString PsqlDatabase::getParcelle(QString hexacle){
    QString query = "select num_parcelle from casage, parcelle "
                    "where st_intersects(parcelle.the_geom, casage.the_geom) and code='"+hexacle+"'";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    if(q.next()){
        return q.value("num_parcelle").toString();
    }
    return QString();
}

QStringList PsqlDatabase::multipleHexacleOccurence(){
    QString query = "select code "
                    "from (select code, count(code) from casage group by code) occurs "
                    "where occurs.count>1";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    QStringList values;
    while(q.next()){
        values<<q.value("code").toString();
    }
    return values;
}


QStringList PsqlDatabase::infoRue(QString voie){
    QString query = "select type_chaussee, anc_chaussee, type_trottoir, anc_trottoir, gestionnaire "
                    "from voirie where voie='"+voie+"'";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }

    if(q.next()){
        return QStringList()<<q.value("type_chaussee").toString()
                    <<q.value("anc_chaussee").toString()
                   <<q.value("type_trottoir").toString()
                  <<q.value("anc_trottoir").toString()
                 <<q.value("gestionnaire").toString();

    }
    return QStringList()<<""<<""<<""<<""<<"";
}

QString PsqlDatabase::getPoche(QString hexacle){
    QString query = "Select poche from poche, casage "
                    "where st_intersects(casage.the_geom, poche.the_geom) "
                    "AND code='"+hexacle+"'";
    QSqlQuery q = psql.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    if(q.next()){
        return q.value("poche").toString();
    }
    return QString();
}
