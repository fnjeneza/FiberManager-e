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
    QSqlQuery executeQuery(QSqlDatabase qsd, QString query);
    void closeDb(QSqlDatabase);

    QString optique;
    QSqlDatabase optiqueDb;

};

#endif // DBHANDLER_H
