#include "parameters.h"

Parameters::Parameters()
{

}

Parameters::~Parameters()
{

}

QString Parameters::getNro(){
    return nro;
}

void Parameters::setNro(QString nro){
    this->nro=nro;
}
