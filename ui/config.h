#ifndef CONFIG_H
#define CONFIG_H

#include <QWidget>
#include <QFileDialog>
#include "handler/confighandler.h"

namespace Ui {
class Config;
}

class Config : public QWidget
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

    void remove(QString project_id);


    FMConfigDAO getConfig() const;
    void setConfig(const FMConfigDAO &value);

    bool isSet();

private slots:
    void on_connect_clicked();

    void on_newConfig_clicked();

    void on_save_clicked();

    void on_deleteConfig_clicked();

    void on_browseOptique_clicked();

    void on_editConfig_clicked();

private:
    Ui::Config *ui;
    ConfigHandler *configHandler;
    FMConfigDAO config;

};

#endif // CONFIG_H
