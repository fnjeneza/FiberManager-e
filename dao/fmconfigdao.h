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



    QString getProjectId() const;
    void setProjectId(const QString &value);

    QString getNro() const;
    void setNro(const QString &value);

    QString getPlaque() const;
    void setPlaque(const QString &value);

    QString getExt() const;
    void setExt(const QString &value);

    QString getBase() const;
    void setBase(const QString &value);

    QString getHost() const;
    void setHost(const QString &value);

    QString getPort() const;
    void setPort(const QString &value);

    QString getUserName() const;
    void setUserName(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    QString getOptique() const;
    void setOptique(const QString &value);

    QString getInfra() const;
    void setInfra(const QString &value);

    QString getSite() const;
    void setSite(const QString &value);

private:
    QString projectId;
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
