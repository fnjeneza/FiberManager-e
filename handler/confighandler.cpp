#include "confighandler.h"

ConfigHandler::ConfigHandler()
{
    connect();
}

ConfigHandler::~ConfigHandler()
{
    close();
}

void ConfigHandler::connect(){
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("config.db");

    if(!database.open()){
        qDebug()<<database.lastError().text();
    }
}

QStringList ConfigHandler::retrieveAllProjectId(){
    QString query = "SELECT project_id FROM connexion";
    QSqlQuery response = database.exec(query);
    if(!response.isActive()){
        qDebug()<<response.lastError().text();
        query = "CREATE TABLE connexion ("
                "project_id     text primary key,"
                "nro            varchar(5),"
                "plaque         varchar(9),"
                "ext            varchar(5),"
                "db_name        text,"
                "host           text,"
                "port           text,"
                "username       text,"
                "password       text,"
                "optique        text,"
                "infra          text,"
                "site           text)";
        response = database.exec(query);
        qDebug()<<response.lastError().text();
    }
    QStringList configs;
    while(response.next()){
        configs<<response.value(0).toString();
    }
    return configs;
}

void ConfigHandler::close(){
    database.close();
}

void ConfigHandler::addConfig(FMConfigDAO config){

    QString query = "INSERT INTO connexion (project_id, nro,plaque, ext, db_name, host, port,"
                    "username, password, optique, infra, site) VALUES('"+config.getProjectId()+"','"+config.getNro()+"',"
                    "'"+config.getPlaque()+"','"+config.getExt()+"','"+config.getBase()+"','"+config.getHost()+"',"
                    "'"+config.getPort()+"','"+config.getUserName()+"','"+config.getPassword()+"','"+config.getOptique()+"',"
                    "'"+config.getInfra()+"','"+config.getSite()+"')";
    QSqlQuery q = database.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
}

bool ConfigHandler::exists(FMConfigDAO config){
    QString query = "SELECT project_id FROM connexion WHERE project_id='"+config.getProjectId()+"'";
    QSqlQuery q = database.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
    if(q.next())
        return true;
    return false;
}

void ConfigHandler::removeConfig(FMConfigDAO config){
    QString query = "DELETE FROM connexion WHERE project_id='"+config.getProjectId()+"'";
    QSqlQuery q = database.exec(query);
    if(!q.isActive()){
        qDebug()<<q.lastError().text();
    }
}
