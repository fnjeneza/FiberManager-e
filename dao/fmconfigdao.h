#ifndef FMCONFIGDAO_H
#define FMCONFIGDAO_H

#include <QString>

class FMConfigDAO
{
public:
    FMConfigDAO();
    FMConfigDAO(QString projectId, QString nro, QString plaque, QString ext,QString base, QString host, QString port,
                QString userName, QString password, QString optique, QString infra, QString site);
    ~FMConfigDAO();

    QString getProjectId();
    QString getNro();
    QString getPlaque();
    QString getExt();
    QString getHost();
    QString getPort();
    QString getUserName();
    QString getPassword();
    QString getOptique();
    QString getInfra();
    QString getSite();
    QString getBase();

    void setProjectId(QString projectId);
    void setNro(QString nro);
    void setBase(QString base);
    void setPlaque(QString plaque);
    void setExt(QString ext);
    void setHost(QString host);
    void setPort(QString port);
    void setUserName(QString userName);
    void setPassword(QString password);
    void setOptique(QString optique);
    void setInfra(QString infra);
    void setSite(QString site);

private:
    QString project_id;
    QString nro;
    QString plaque;
    QString ext;
    QString base;
    QString host;
    QString port;
    QString userName;
    QString password;
    QString optique;    //path to optique
    QString infra;      //path to infra
    QString site;       //path to site
};

#endif // FMCONFIGDAO_H
