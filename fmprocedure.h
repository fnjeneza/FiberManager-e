#ifndef FMPROCEDURE_H
#define FMPROCEDURE_H

#include <QDir>
#include <QFile>
#include <QStringList>
#include <QDate>
#include <QTime>
#include "dbhandler.h"


class FMProcedure
{
public:
    FMProcedure();
    ~FMProcedure();
    void generateDoeRepository(QString path, QString nro, QString poche, QString plaque, QString ext);
    void controlDoeRepository(QString path);
    void showMessage(QString message);
    void extractFlr(QString schemas,QString casage);
    void extractSiteFromOptique();
    void setOptiquePath(QString path);
    void extractNoeudsFromOptique();
    void connectToOptiqueMdb();
    void closeConnectionToOptiqueMdb();
    void connectToPostgresServer(QString database, QString host, QString userName, QString pwd);


private:
    QString optiquePath;
    QSqlDatabase optiqueDb;
    QSqlDatabase psqlDb;
};

#endif // FMPROCEDURE_H
