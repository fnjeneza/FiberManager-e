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
#include "parameters.h"


class FMProcedure
{
public:
    FMProcedure();
    ~FMProcedure();
    void generateDoeRepository(QString path, QString nro, QString poche, QString plaque, QString ext);
    void controlDoeRepository(QString path);
    void showMessage(QString message);
    void exportFlr();
    bool isFrontZone(QString num, QString suf, QString voie); //est ce une zone avant


private:
    QString optiquePath;
    QSqlDatabase optiqueDb;
    QSqlDatabase psqlDb;
    DbHandler* default_psql;
    DbHandler* tmp_psql;
};

#endif // FMPROCEDURE_H
