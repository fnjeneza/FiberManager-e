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
    void disconnect();
    void setOptique(QString optique);
    void setInfra(QString infra);
    void setSite(QString site);
    bool siteExists(QString hexacle, QString num,QString suf, QString voie);
    void setLindSite(QString recno, QString no, QString numSect, QString noeud);
    QStringList tables(int schema);
    QString structure(int schema, QString tableName);
    bool needUpdate(QString hexacle, QString num, QString suf, QString voie, int bal, QString x, QString y);
    void addSite(QString recno, QString no, QString hexacle, QString num, QString suf, QString voie, QString code_postal, QString commune, int bal, QString x, QString y);
    void updateSite(QString hexacle, QString num, QString suf, QString voie, QString code_postal, QString commune, int bal, QString x, QString y);
    QString nextRecno(QString tableName);
    QString nextSiteNumber();
    QStringList retrieveRivoliRecno();
    bool isZoneAvant(QString num, QString suf, QString voie);
    bool isVoieExists(QString voie);
    QStringList getAssociatedChambre(QString noeud);
    QString getInfraNoeudAdresse(QString noeud);
    QString getOptiqueNoeudAdresse(QString noeud);
    QStringList getAllOptiqueNoeud();
    QString controlNommage(QString plaque, QString noeud);
    QStringList getAllSite();
    QStringList getCableAval(QString noeud);
    QString getCableAmont(QString noeud);
    QString getTranche(QString noeud);
    bool cableHasCorrectExtremite(QString noeud);
    bool cableHasOrigine(QString noeud);
    QString getNoeudBis(QString noeud);

private:
    QSqlDatabase infra_db;
    QSqlDatabase optique_db;
    QSqlDatabase site_db;

    QString infra;
    QString optique;
    QString site;

    QMap<QString,QString> rivoli;
    bool isRivoliLoaded=false;


    QString lastRecno(QString tableName);
    QString lastSiteNumber();
    void loadRivoli();
};

#endif // MDBHANDLER_H
