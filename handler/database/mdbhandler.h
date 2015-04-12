#ifndef MDBHANDLER_H
#define MDBHANDLER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>

class MdbHandler
{
public:
    MdbHandler();
    ~MdbHandler();

    void connect(QString ConnectionName);
    void connect();
    void setOptique(QString optique);
    void setInfra(QString infra);
    void setSite(QString site);
    bool siteExists(QString hexacle, QString num,QString suf, QString voie);
    QStringList tables(int schema);
    QString structure(int schema, QString tableName);
    bool needUpdate(QString hexacle, QString num, QString suf, QString voie, int bal, QString x, QString y);
    void addSite(QString recno, QString no, QString hexacle, QString num, QString suf, QString voie, QString code_postal, QString commune, int bal, QString x, QString y);
    void updateSite(QString hexacle, QString num, QString suf, QString voie, QString code_postal, QString commune, int bal, QString x, QString y);
    QString nextRecno(QString tableName);
    QString nextSiteNumber();

private:
    QSqlDatabase infra_db;
    QSqlDatabase optique_db;
    QString infra;
    QString optique;
    QString site;

    QString lastRecno(QString tableName);
    QString lastSiteNumber();
};

#endif // MDBHANDLER_H
