#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QSqlDatabase>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "dao/fmconfigdao.h"

class ConfigHandler
{
public:
    ConfigHandler();
    ~ConfigHandler();

    QStringList retrieveAllProjectId();
    FMConfigDAO getConfig(QString project_id);
    void addConfig(FMConfigDAO);
    void removeConfig(FMConfigDAO);
    bool exists(FMConfigDAO);

private:
    void connect();
    void close();

    QSqlDatabase database;


};

#endif // CONFIGHANDLER_H
