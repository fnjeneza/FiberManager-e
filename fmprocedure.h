#ifndef FMPROCEDURE_H
#define FMPROCEDURE_H

#include <QDir>
#include <QFile>
#include <QStringList>
#include <QDate>
#include <QTime>
#include <QSqlRecord>
#include <QFile>
#include "dbhandler.h"
#include "address.h"


class FMProcedure
{
public:
    FMProcedure();
    ~FMProcedure();
    void generateDoeRepository(QString path, QString nro, QString poche, QString plaque, QString ext);
    void controlDoeRepository(QString path);
    void showMessage(QString message);
    void exportFlr();
    void extractSiteFromOptique();
    void setOptiquePath(QString path);
    void extractNoeudsFromOptique();
    void connectToOptiqueMdb();
    void closeConnectionToOptiqueMdb();


private:
    QString optiquePath;
    QSqlDatabase optiqueDb;
    QSqlDatabase psqlDb;
};

#endif // FMPROCEDURE_H
