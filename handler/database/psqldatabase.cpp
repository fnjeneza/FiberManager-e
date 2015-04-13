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

    if(!psql.open()){
        qDebug()<< psql.lastError().text();
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
    QString query  = "select code, num, suf, voie from flr";
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

QStringList PsqlDatabase::adressesWithoutHexacode(){
    QString query = "SELECT num, suf, voie FROM casage where code is null";
    QSqlQuery q = psql.exec(query);
    QStringList values;

    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    while(q.next()){
        values<<"Hexacle non renseigné;"<<q.value("num").toString()+";"+q.value("suf").toString()+";"+q.value("voie").toString();
    }
    return values;

}

QStringList PsqlDatabase::invalidADresses(){
    QString query = "select code from casage where num is null or voie=''";
    QSqlQuery q = psql.exec(query);
    QStringList values;
    if(!q.isValid()){
        qDebug()<<q.lastError().text();
    }
    while(q.next()){
        values<<"Adresse incorrecte;"<<q.value("code").toString();
    }
    return values;
}






