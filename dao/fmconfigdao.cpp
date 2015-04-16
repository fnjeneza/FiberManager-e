#include "fmconfigdao.h"

FMConfigDAO::FMConfigDAO()
{

}

FMConfigDAO::FMConfigDAO(QString projectId, QString nro, QString plaque, QString ext,QString base, QString host, QString port, QString userName, QString password, QString optique, QString infra, QString site){
    this->projectId = projectId;
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
QString FMConfigDAO::getProjectId() const
{
    return projectId;
}

void FMConfigDAO::setProjectId(const QString &value)
{
    projectId = value;
}
QString FMConfigDAO::getNro() const
{
    return nro;
}

void FMConfigDAO::setNro(const QString &value)
{
    nro = value;
}
QString FMConfigDAO::getPlaque() const
{
    return plaque;
}

void FMConfigDAO::setPlaque(const QString &value)
{
    plaque = value;
}
QString FMConfigDAO::getExt() const
{
    return ext;
}

void FMConfigDAO::setExt(const QString &value)
{
    ext = value;
}
QString FMConfigDAO::getBase() const
{
    return base;
}

void FMConfigDAO::setBase(const QString &value)
{
    base = value;
}
QString FMConfigDAO::getHost() const
{
    return host;
}

void FMConfigDAO::setHost(const QString &value)
{
    host = value;
}
QString FMConfigDAO::getPort() const
{
    return port;
}

void FMConfigDAO::setPort(const QString &value)
{
    port = value;
}
QString FMConfigDAO::getUserName() const
{
    return userName;
}

void FMConfigDAO::setUserName(const QString &value)
{
    userName = value;
}
QString FMConfigDAO::getPassword() const
{
    return password;
}

void FMConfigDAO::setPassword(const QString &value)
{
    password = value;
}
QString FMConfigDAO::getOptique() const
{
    return optique;
}

void FMConfigDAO::setOptique(const QString &value)
{
    optique = value;
}
QString FMConfigDAO::getInfra() const
{
    return infra;
}

void FMConfigDAO::setInfra(const QString &value)
{
    infra = value;
}
QString FMConfigDAO::getSite() const
{
    return site;
}

void FMConfigDAO::setSite(const QString &value)
{
    site = value;
}















