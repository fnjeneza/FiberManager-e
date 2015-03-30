#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>


class DbHandler
{
public:
    DbHandler();
    ~DbHandler();

    QSqlDatabase connectToMdb(QString fullPath, QString connectionName);
    QSqlDatabase connectToPostgresServer(QString connectionName);
    QSqlQuery executeQuery(QSqlDatabase qsd, QString query);
    void closeDb(QSqlDatabase);
    void setDatabase(QString database);
    void setHost(QString host);
    void setUserName(QString userName);
    void setPassword(QString pwd);

    QString optique;
    QSqlDatabase optiqueDb;

private:
    QString database;
    QString host;
    QString userName;
    QString pwd;

};

#endif // DBHANDLER_H
