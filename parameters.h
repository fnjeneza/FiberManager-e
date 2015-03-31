#ifndef PARAMETERS_H
#define PARAMETERS_H

#include<QString>

class Parameters
{
public:
    Parameters();
    ~Parameters();

    static QString getNro();
    static void setNro();
private:
    QString nro;
};

#endif // PARAMETERS_H
