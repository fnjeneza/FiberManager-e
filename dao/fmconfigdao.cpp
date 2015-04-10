#include "fmconfigdao.h"

FMConfigDAO::FMConfigDAO()
{

}

FMConfigDAO::FMConfigDAO(QString projectId, QString nro, QString plaque, QString ext,QString base, QString host, QString port, QString userName, QString password, QString optique, QString infra, QString site){
    this->project_id = projectId;
    this->nro = nro;
    this->plaque = plaque;
    this->ext = ext;
    this->host = host;
    this->port = port;
    this->userName = userName;
    this->password = password;
    this->optique = optique;
    this->infra = infra;
    this->site = site;
    this->base = base;
}

FMConfigDAO::~FMConfigDAO()
{

}

QString FMConfigDAO::getProjectId(){
    return project_id;
}

QString FMConfigDAO::getNro(){
    return nro;
}

QString FMConfigDAO::getPlaque(){
    return plaque;
}

QString FMConfigDAO::getExt(){
    return ext;
}

QString FMConfigDAO::getHost(){
    return host;
}

QString FMConfigDAO::getPort(){
    return port;
}

QString FMConfigDAO::getUserName(){
    return userName;
}

QString FMConfigDAO::getPassword(){
    return password;
}

QString FMConfigDAO::getOptique(){
    return optique;
}

QString FMConfigDAO::getInfra(){
    return infra;
}

QString FMConfigDAO::getSite(){
    return site;
}

QString FMConfigDAO::getBase(){
    return base;
}

void FMConfigDAO::setProjectId(QString projectId){
    this->project_id=projectId;
}

