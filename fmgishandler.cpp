#include "fmgishandler.h"

FMGisHandler::FMGisHandler()
{
    CODELESS="select * from codeless_view";
    ALL_ADDRESSES="select code, num, suf, voie from casage";
    DbHandler dbh=DbHandler("10.0.0.254","be_free","123456","PLA13_038");
    //gisDb = dbh.connectToPostgresServer("gis");
    audit=new QFile(QDir::tempPath()+"/audit.csv");
    if(audit->exists()){
        audit->remove();
    }

}

FMGisHandler::~FMGisHandler()
{
    if(audit->isOpen()){
        audit->close();
    }
    delete audit;
}

void FMGisHandler::codeController(){
    if(!gisDb.isOpen()){
        qDebug()<<"database not opened";
        qDebug()<<gisDb.lastError().text();
        return;
    }
    audit->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QSqlQuery query = gisDb.exec(CODELESS);
    if(!query.isActive()){
        qDebug()<<query.lastError().text();
        return;
    }

    QTextStream text(audit);
    while(query.next()){
        QString adresse;
//        = Address::getCompleteAddress(query.value("num").toInt(),
//                                    query.value("suf").toString(),
//                                    query.value("voie").toString());
        text<<";Adresse sans hexacle;"+adresse+"\n";
        text.flush();
    }
    audit->close();

}

void FMGisHandler::invalidAddress(){
    if(!gisDb.isOpen()){
        qDebug()<<"database not opened";
        qDebug()<<gisDb.lastError().text();
        return;
    }

    audit->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QSqlQuery query = gisDb.exec(ALL_ADDRESSES);
    if(!query.isActive()){
        qDebug()<<query.lastError().text();
        return;
    }
    QTextStream text(audit);
    while(query.next()){
        QString addresse;
//        = Address::getCompleteAddress(query.value("num").toInt(),
//                                                       query.value("suf").toString(),
//                                                       query.value("voie").toString());
        QString num1=QString::number(query.value("num").toInt());
        QString num2=query.value("num").toString();
        qDebug()<<num1<<" "<<num2;
        if(num1.compare(num2)!=0){
            text<<";Num voie incorrect;"+query.value("code").toString()+";"+addresse+"\n";
            text.flush();
        }

    }
    audit->close();

}
