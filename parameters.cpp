#include "parameters.h"

Parameters::Parameters()
{
    host="192.168.0.126";
    databaseName="PLA13_038";
    userName="be_free";
    password="123456";
}

Parameters::~Parameters()
{

}


QString Parameters::getDatabaseName(){
    return databaseName;
}

QString Parameters::getHost(){
    return host;
}

QString Parameters::getPassword(){
    return password;
}

QString Parameters::getUserName(){
    return userName;
}

void Parameters::setNro(QString nro){
    this->nro=nro;
}


