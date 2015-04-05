#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>


class DbHandler
{
public:
    DbHandler(QString mdbPath); //connect to access db
    DbHandler(QString host, QString userName, QString password, QString databaseName);
    DbHandler(QString connectionName, QString host, QString userName, QString password, QString databaseName);//connect to postgres db
    ~DbHandler();

    QSqlQuery executeQuery(int BASE, QString query);
    void closeDb();

    static const int BASE_MDB=1; //mdb
    static const int BASE_PSQL=2;   //psql

private:
    QSqlDatabase mdb;
    QSqlDatabase psql;
    QSqlQuery req;

};

#endif // DBHANDLER_H
