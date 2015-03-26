#include "dbhandler.h"

DbHandler::DbHandler()
{
    optique = "T:\\Secteur.mdb";
}

DbHandler::~DbHandler()
{

}

QSqlDatabase DbHandler::connectToMdb(QString fullPath, QString connectionName){
    QSqlDatabase qsd = QSqlDatabase::addDatabase("QODBC", connectionName);
    qsd.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+fullPath);
    if(!qsd.open()){
            qDebug()<<qsd.lastError().text();
    }

    return qsd;
}

void DbHandler::closeDb(QSqlDatabase qsd){
    if(qsd.isOpen()){
       qsd.close();
    }
}

QSqlQuery DbHandler::executeQuery(QSqlDatabase qsd, QString query){
    QSqlQuery qsq = qsd.exec(query);
    if(!qsq.isValid()){
        qsd.lastError().text();
    }
    return qsq;

}
