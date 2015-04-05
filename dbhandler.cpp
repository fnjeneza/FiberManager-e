#include "dbhandler.h"

DbHandler::DbHandler(QString host, QString userName, QString password, QString databaseName)
{


    psql=QSqlDatabase::addDatabase("QPSQL","default_psql");
    psql.setHostName(host);
    psql.setUserName(userName);
    psql.setPassword(password);
    psql.setDatabaseName(databaseName);

    if(!psql.open()){
        qDebug()<<psql.lastError().text();
    }

}

DbHandler::DbHandler(QString connectionName,QString host, QString userName, QString password, QString databaseName)
{


    psql=QSqlDatabase::addDatabase("QPSQL",connectionName);
    psql.setHostName(host);
    psql.setUserName(userName);
    psql.setPassword(password);
    psql.setDatabaseName(databaseName);

    if(!psql.open()){
        qDebug()<<psql.lastError().text();
    }

}

DbHandler::DbHandler(QString mdbPath){
    mdb=QSqlDatabase::addDatabase("QODBC","default_mdb");
    mdb.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ="+mdbPath);
    if(!mdb.open()){
        qDebug()<<mdb.lastError().text();
    }
}

DbHandler::~DbHandler()
{
    closeDb();
}



void DbHandler::closeDb(){
    if(psql.isOpen()){
       psql.close();
    }
    if(mdb.isOpen()){
        mdb.close();
    }
}

QSqlQuery DbHandler::executeQuery(int BASE, QString query){
    switch (BASE) {
    case 1:
        req = mdb.exec(query);
        break;
    case 2:
        req = psql.exec(query);
        break;
    }
    if(!req.isActive()){
        qDebug()<<req.lastError().text();
    }

    return req;

}

