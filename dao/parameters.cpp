#include "parameters.h"

Parameters::Parameters()
{
    projectId.clear();
    nro.clear();
    plaque.clear();
    ext.clear();
    base.clear();
    host.clear();
    port.clear();
    userName.clear();
    password.clear();
    optique.clear();    //path to optique
    infra.clear();      //path to infra
    site.clear();       //path to site
}

Parameters::Parameters(QString projectId, QString nro, QString plaque, QString ext,QString base, QString host, QString port, QString userName, QString password, QString optique, QString infra, QString site){
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

Parameters::~Parameters()
{

}
QString Parameters::getProjectId() const
{
    return projectId;
}

void Parameters::setProjectId(const QString &value)
{
    projectId = value;
}
QString Parameters::getNro() const
{
    return nro;
}

void Parameters::setNro(const QString &value)
{
    nro = value;
}
QString Parameters::getPlaque() const
{
    return plaque;
}

void Parameters::setPlaque(const QString &value)
{
    plaque = value;
}
QString Parameters::getExt() const
{
    return ext;
}

void Parameters::setExt(const QString &value)
{
    ext = value;
}
QString Parameters::getBase() const
{
    return base;
}

void Parameters::setBase(const QString &value)
{
    base = value;
}
QString Parameters::getHost() const
{
    return host;
}

void Parameters::setHost(const QString &value)
{
    host = value;
}
QString Parameters::getPort() const
{
    return port;
}

void Parameters::setPort(const QString &value)
{
    port = value;
}
QString Parameters::getUserName() const
{
    return userName;
}

void Parameters::setUserName(const QString &value)
{
    userName = value;
}
QString Parameters::getPassword() const
{
    return password;
}

void Parameters::setPassword(const QString &value)
{
    password = value;
}
QString Parameters::getOptique() const
{
    return optique;
}

void Parameters::setOptique(const QString &value)
{
    optique = value;
}
QString Parameters::getInfra() const
{
    return infra;
}

void Parameters::setInfra(const QString &value)
{
    infra = value;
}
QString Parameters::getSite() const
{
    return site;
}

void Parameters::setSite(const QString &value)
{
    site = value;
}

















