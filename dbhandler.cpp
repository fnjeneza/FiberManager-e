#include "dbhandler.h"

DbHandler::DbHandler()
{
    optique = "T:\\Secteur.mdb";
    database = "PLA13_038";
    host="10.0.0.254";
    userName="be_free";
    pwd="123456";
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

QSqlDatabase DbHandler::connectToPostgresServer(QString connectionName){
    QSqlDatabase qsd =QSqlDatabase::addDatabase("QPSQL", connectionName);
    qsd.setUserName(userName);
    qsd.setPassword(pwd);
    qsd.setHostName(host);
    qsd.setDatabaseName(database);

    if(!qsd.open()){
        qDebug()<< qsd.lastError().text();
        qDebug() << "Connexion à la base non réalisée";
    }
    return qsd;
}

void DbHandler::setDatabase(QString database){
    this->database=database;
}

void DbHandler::setHost(QString host){
    this->host=host;
}

void DbHandler::setUserName(QString userName){
    this->userName=userName;
}

void DbHandler::setPassword(QString pwd){
    this->pwd=pwd;
}
