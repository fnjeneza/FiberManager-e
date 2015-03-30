#ifndef ADDRESS_H
#define ADDRESS_H

#include <QString>
#include <QStringList>
#include <QDebug>

class Address
{
public:
    Address(QString address);
    Address(int addressNum, QString addressSuf, QString addressVoie);

    QString getCompleteAddress();
    static QString getCompleteAddress(int num, QString suf, QString voie);
    int getAddressNum();
    void setAddressNum(int num);
    QString getAddressSuf();
    void setAddressSuf(QString suf);
    QString getAddressVoie();
    void setAddressVoie(QString voie);
    bool isValid();


private:
    int addressNum;
    QString addressSuf;
    QString addressVoie;
    bool valid;
};

#endif // ADDRESS_H
