#include <QCoreApplication>
#include "fmprocedure.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    bool extractOptique=false;
    bool extractFlr=false;
    QString extractionType; //optique, infra, netgeo, flr
    QString path;
    QStringList arguments = QCoreApplication::arguments();

    for(int i=0;i<arguments.count();i++){
        if(arguments.at(i)=="-e"){
            extractionType=arguments.at(++i);
            if(extractionType.toLower()=="optique"){
                extractOptique=true;
            }
            if(extractionType.toLower()=="flr"){
                extractFlr=true;
            }
        }
        if(arguments.at(i)=="-d"){
            path=arguments.at(++i);
        }
    }
    FMProcedure fmp=FMProcedure();
    /** Genere le dossier DOE*/
    //fmp.generateDoeRepository("T:/dossier","CRI13","28","PLA13_014","EXT01");

    /** Contrôle le DOE non implémenter*/
    //fmp.controlDoeRepository("T:/dossier/501_DOE_CTR23_CRI13_EXT01_150220_INDA");

    /** Test connection et extraction de l'optique MDB*/
    if(extractOptique){
        fmp.setOptiquePath(path);
        fmp.connectToOptiqueMdb();
        fmp.extractNoeudsFromOptique();
        fmp.closeConnectionToOptiqueMdb();
    }

    if(extractFlr){
        //fmp.exportFlr();
    }



qDebug()<< "Fini";
    return a.exec();
}
