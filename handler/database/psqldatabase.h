#ifndef PSQLDATABASE_H
#define PSQLDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QProgressDialog>


class PsqlDatabase
{
public:
    PsqlDatabase();
    ~PsqlDatabase();

    void connect();
    QString flr();
    QStringList columns(QString tableName);
    QStringList lr(QString hexacle, QString num, QString suf, QString voie);
    QStringList retrieveAdresses();
    QStringList adressesWithoutHexacode();
    QStringList invalidADresses();


    QString getHost() const;
    void setHost(const QString &value);

    QString getDatabaseName() const;
    void setDatabaseName(const QString &value);

    QString getUserName() const;
    void setUserName(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    int getPort() const;
    void setPort(int value);

    QString getNro() const;
    void setNro(const QString &value);

private:
    QSqlDatabase psql;

    QString host;
    int port;
    QString databaseName;
    QString userName;
    QString password;
    QString nro;
};

#endif // PSQLDATABASE_H
