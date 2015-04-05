#include "fmprocedure.h"

FMProcedure::FMProcedure()
{
    Parameters p = Parameters();
    default_psql=new DbHandler(p.getHost(),
                       p.getUserName(),
                       p.getPassword(),
                       p.getDatabaseName());
    tmp_psql=new DbHandler("tmp",
                           p.getHost(),
                       p.getUserName(),
                       p.getPassword(),
                       p.getDatabaseName());
}


FMProcedure::~FMProcedure()
{   default_psql->closeDb();
    tmp_psql->closeDb();
    delete default_psql;
    delete tmp_psql;
}



void FMProcedure::showMessage(QString message){
    qDebug()<<message;
}

void FMProcedure::generateDoeRepository(QString path, QString nro, QString poche, QString plaque, QString ext){
    QDate current=QDate::currentDate();
    QString date=QString::number(current.year()).right(2)+QString::number(current.month())+QString::number(current.day());

    QDir root(path);
    if(!root.exists()){
       showMessage("Repertoire non trouvé");
        return;
    }

    QString rep_501;
    if(ext.isEmpty()){
        rep_501="501_DOE_CTR"+poche+"_"+nro+"_"+ext+"_"+date+"_INDA";
    }
    else{
        rep_501="501_DOE_CTR"+poche+"_"+nro+"_"+date+"_INDA";
    }
    QStringList arborescence;
    arborescence.append(rep_501+"/415_FAT_TVX_"+nro);
    arborescence.append(rep_501+"/515_BASE_CTR"+poche+"_DOE_"+nro+"_INDA");
    arborescence.append(rep_501+"/521_PHOTOS_CTR"+poche+"_TVX_"+nro+"_INDA");
    arborescence.append(rep_501+"/530_INF_GC_PLN_TVX_"+nro);
    arborescence.append(rep_501+"/550_DOSS_INFRA_TVX_"+nro+"_INDA");
    arborescence.append(rep_501+"/580_DM_TVX_"+nro+"_INDA/581_DM_TVX_CTR"+poche+"_"+nro+"_INDA/CTR_"+plaque+"_"+poche+"_INDA");
    arborescence.append(rep_501+"/591_JUSTIFICATION_DOE_CTR"+poche+"_"+nro+"_"+date+"_INDA");

    for(int i=0; i<arborescence.size(); i++){
        root.mkpath(arborescence.at(i));
    }
}

void FMProcedure::controlDoeRepository(QString path){

}

bool FMProcedure::isFrontZone(QString num, QString suf, QString voie){

    QString num_voie=num+" "+suf;
    QString req = "select rivoli from site.rivoli where voie='"+voie+"'";

    QSqlQuery query = tmp_psql->executeQuery(DbHandler::BASE_PSQL, req);
    if(!query.next()){
        return false;
    }

    QString rivoli=query.value(0).toString();
    req = "select recno from optique.noeuds where num_voie='"+num_voie.trimmed()+"' and rivoli='"+rivoli+"' and deleted<>'*'";
    query = tmp_psql->executeQuery(DbHandler::BASE_PSQL, req);
    if(!query.next()){
        return false;
    }
    if(!query.value(0).toString().isEmpty()){
        return true;
    }
    return false;
}




void FMProcedure::exportFlr(){
    QSqlQuery query = default_psql->executeQuery(DbHandler::BASE_PSQL,"select * from flr");
    if(!query.isActive()){
        qDebug()<< query.lastError().text();
        return;
    }


    QFile csv(QDir::tempPath()+"/flr.csv");
    csv.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&csv);
    out<<"hexacle;adresse;nb_bal;phd;parcelle;zone;type_habitat;type_chausse;anc_chaussee;type_trottoir;anc_trottoir;gestionnaire;nro;poche;boite;adduction;concessionnaire;pm;lr_pm;commentaire\n";
    csv.flush();



    while(query.next()){
        QString hexacle = query.value("code").toString();
        QString bal = query.value("num").toString();
        QString num_parc = query.value("num_parcelle").toString();
        QString type_chaussee = query.value("type_chaussee").toString();
        QString anc_chaussee = query.value("anc_chaussee").toString();
        QString type_trottoir = query.value("type_trottoir").toString();
        QString anc_trottoir = query.value("anc_trottoir").toString();
        QString gestionnaire = query.value("gestionnaire").toString();
        QString poche = query.value("poche").toString();
        QString boite_rattachement = query.value("boite_rattachement").toString().replace(";",",");
        QString adduction = query.value("adduction").toString();
        QString concessionnaire = query.value("concessionnaire").toString();
        QString reference = query.value("reference").toString().replace(";",",");
        QString lr_pm = query.value("logements_pm").toString();
        QString commentaire = query.value("commentaire").toString().replace(";",",");
        QString adresse = Address::getCompleteAddress(query.value("num").toInt(),
                                                      query.value("suf").toString(),
                                                      query.value("voie").toString(),
                                                      query.value("code_postal").toString(),
                                                      query.value("ville").toString());

        QString zone="Zone Arriere";
        bool isZone = isFrontZone(query.value("num").toString().trimmed(),
                                    query.value("suf").toString().trimmed(),
                                    query.value("voie").toString().trimmed());
        if(isZone){
            zone="Zone Avant";
        }
        QString nro="SLO13";
        QString phd="PHD";
        QString type_habitat=bal.toInt()>1?"COLL":"INDIV";

        if(reference.isEmpty()){
            reference="PAS DE PM";
        }
        if(adduction.isEmpty()){
            adduction="GC";
            concessionnaire="FT";
        }
        if(concessionnaire.isEmpty()){
            concessionnaire="FT";
        }
        out<<hexacle+";"+adresse +";"+bal+";"+phd+";"+num_parc+";"+zone+";"+type_habitat +";"+type_chaussee+";"
          << anc_chaussee +";"+type_trottoir +";"+anc_trottoir +";"+gestionnaire +";"+nro+";"+ poche+";"+boite_rattachement+";"+ adduction +";"
          << concessionnaire+";"+ reference+";"+ lr_pm+";"+ commentaire+"\n";
        csv.flush();
    }
    csv.close();
}
