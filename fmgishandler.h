#ifndef FMGISHANDLER_H
#define FMGISHANDLER_H

#include <QFile>
#include <QDir>
#include "dbhandler.h"

class FMGisHandler
{
public:
    FMGisHandler();
    ~FMGisHandler();

    void codeController();
    void invalidAddress();

private:
    QSqlDatabase gisDb;
    QFile* audit;

    QString CODELESS;
    QString ALL_ADDRESSES;
};

#endif // FMGISHANDLER_H
