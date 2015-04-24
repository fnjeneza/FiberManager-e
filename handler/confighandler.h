#ifndef CONFIGHANDLER_H
#define CONFIGHANDLER_H

#include <QSqlDatabase>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "dao/parameters.h"

class ConfigHandler
{
public:
    ConfigHandler();
    ~ConfigHandler();

    QStringList retrieveAllProjectId();
    Parameters getConfiguration(QString project_id);
    void addConfig(Parameters);
    void removeConfig(Parameters);
    bool exists(Parameters);
    void update(Parameters);

private:
    void connect();
    void close();

    QSqlDatabase database;


};

#endif // CONFIGHANDLER_H
