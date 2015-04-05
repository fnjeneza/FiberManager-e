#ifndef PARAMETERS_H
#define PARAMETERS_H

#include<QString>

class Parameters
{
public:
    Parameters();
    ~Parameters();

     QString getNro();
     QString getHost();
     QString getUserName();
     QString getPassword();
     QString getDatabaseName();
     void setNro(QString);

private:
     void initialize();
     QString nro;
     QString host;
     QString userName;
     QString password;
     QString databaseName;

};

#endif // PARAMETERS_H
