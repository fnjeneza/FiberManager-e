#include "address.h"

Address::Address(QString address)
{
    address.replace("  ", " ");
    address=address.trimmed();
    valid=true;
    QStringList splitted=address.split(" ");
    QString num_s = splitted.at(0);
    int index;
    addressNum = num_s.toInt();
    if( QString::number(addressNum).compare(num_s)!=0){
        qDebug()<<"Adresse sans numéro de voie valide";
        valid=false;
    }
    if(!valid){
        return;
    }
    QString suf = splitted.at(1);
    if(suf.size()==1){
        addressSuf=suf;
        index=2;
    }
    else{
        addressSuf.clear();
        index=1;
    }
    for(int i=index; i<splitted.size();i++){
        addressVoie=addressVoie+" "+splitted.at(i);
    }
    addressVoie=addressVoie.trimmed();

}

Address::Address(int addressNum, QString addressSuf, QString addressVoie){
    this->addressNum=addressNum;
    this->addressSuf=addressSuf;
    this->addressVoie=addressVoie;

    valid=true;
}

int Address::getAddressNum(){
    return addressNum;
}
QString Address::getAddressSuf(){
    return addressSuf;
}

QString Address::getAddressVoie(){
    return addressVoie;
}

void Address::setAddressNum(int num){
    addressNum = num;
}

void Address::setAddressSuf(QString suf){
    addressSuf=suf;
}

void Address::setAddressVoie(QString voie){
    addressVoie=voie;
}

bool Address::isValid(){
    return valid;
}

QString Address::getCompleteAddress(){
    QString address=QString::number(addressNum);
    if(!addressSuf.isEmpty()){
        address=address+" "+addressSuf;
    }
    address=address+" "+addressVoie;
    return address;
}

QString Address::getCompleteAddress(int num, QString suf, QString voie){
    QString address=QString::number(num);
    if(!suf.isEmpty()){
        address=address+" "+suf;
    }
    address=address+" "+voie;
    return address;
}

QString Address::getCompleteAddress(int num, QString suf, QString voie, QString code_postal, QString commune){
    QString address=QString::number(num);
    if(!suf.isEmpty()){
        address=address+" "+suf;
    }
    address=address+" "+voie+" "+code_postal+" "+commune;
    address=address.trimmed();
    return address;
}
