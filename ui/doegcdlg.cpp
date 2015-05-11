#include "doegcdlg.h"
#include "ui_doegcdlg.h"

DOEGCdlg::DOEGCdlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DOEGCdlg)
{
    ui->setupUi(this);
}

DOEGCdlg::~DOEGCdlg()
{
    delete ui;
}

void DOEGCdlg::on_create_clicked()
{
    QString path = ui->path->text();
    QString XXX00 = ui->xxx00->text();
    QString PLA00_000=ui->pla00_000->text();
    QString EXTXX = ui->extxx->text();
    QString INDX = ui->indx->text();
    QString XX = ui->xx->text();

    QString AAMMJJ;
    QDate current= QDate::currentDate();
    AAMMJJ = QString::number(current.year()).right(2);
    AAMMJJ += QString("0"+QString::number(current.month())).right(2);;
    AAMMJJ += QString("0"+QString::number(current.day())).right(2);

    QString tvx_500 = "500_DOEGC_"+XXX00+"_"+AAMMJJ+"_"+INDX;
    if(!EXTXX.isEmpty()){
        tvx_500 = "500_DOEGC_"+XXX00+"_"+EXTXX+"_"+AAMMJJ+"_"+INDX;
    }
//    QString tvx_415="415_FAT_TVX_"+XXX00;
//    QString tvx_521="521_PHOTOS_CTR"+XX+"_TVX_"+XXX00+"_"+INDX;
//    QString tvx_530="530_INF_GC_PLN_TVX_"+XXX00;
//    QString tvx_550="550_DOSS_INFRA_TVX_"+XXX00+"_"+INDX;
//    QString tvx_560="560_DC_RECOLEMENT_"+XXX00+"_"+INDX;
//    QString tvx_570="570_AUTO_ADMIN_TVX_"+XXX00+"_"+INDX;
//    QString tvx_590="590_JUSTIFICATION_DOEGC_"+XXX00+"_"+AAMMJJ+"_"+INDX;

    //arborescence
    QStringList tvx;
    tvx<<"415_FAT_TVX_"+XXX00;
    tvx<<"521_PHOTOS_CTR"+XX+"_TVX_"+XXX00+"_"+INDX;
    tvx<<"530_INF_GC_PLN_TVX_"+XXX00;
    tvx<<"550_DOSS_INFRA_TVX_"+XXX00+"_"+INDX;
    tvx<<"560_DC_RECOLEMENT_"+XXX00+"_"+INDX;
    tvx<<"570_AUTO_ADMIN_TVX_"+XXX00+"_"+INDX;
    tvx<<"590_JUSTIFICATION_DOEGC_"+XXX00+"_"+AAMMJJ+"_"+INDX;


    QDir root(path);
    root.mkdir(tvx_500); //create the directory

    root.cd(tvx_500);   //change directory

    for(int i=0; i<tvx.length();i++){
        root.mkdir(tvx.at(i));
    }
    this->close();
}

void DOEGCdlg::on_browse_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Selectionner un dossier"),
                                                    "~",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->path->setText(dir);
}
