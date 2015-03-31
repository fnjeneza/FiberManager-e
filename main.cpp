#include <QCoreApplication>
#include "fmprocedure.h"
#include "fmgishandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    bool extractOptique=false;
    bool extractFlr=false;
    bool controlCode=true;
    bool controlAddress=true;
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
    FMGisHandler fmg = FMGisHandler();
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
        DbHandler dbh = DbHandler();
        QSqlDatabase base = dbh.connectToPostgresServer("site");
        QSqlDatabase base2 = dbh.connectToPostgresServer("second");
        QSqlQuery query = base.exec("select distinct boite_rattachement from casage ");
        while(query.next()){
//            QString bpi = query.value(0).toString().trimmed();
//            int num = bpi.right(6).left(3).toInt();
//            int poche = bpi.right(10).left(4).toInt();
//            if(num>95){
//                QString p = "000"+QString::number(poche);
//                QString boite = "PDB_PLA13_038_"+p.right(2)+"_"+QString::number(num-1);
//                QString q = "update casage set boite_rattachement='"+boite+"' where boite_rattachement='"+query.value(0).toString()+"'";
//                QSqlQuery qq =base2.exec(q);
//                qDebug()<<qq.lastError().text();
//            }

        }
        base.close();
        base2.close();
    }
    if(controlCode){
        //fmg.codeController();
    }
    if(controlAddress){
        //fmg.invalidAddress();
    }

qDebug()<< "Fini";
    return a.exec();
}
