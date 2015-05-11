#ifndef DOEGCDLG_H
#define DOEGCDLG_H

#include <QDialog>
#include <QDate>
#include <QDir>
#include <QFileDialog>

namespace Ui {
class DOEGCdlg;
}

class DOEGCdlg : public QDialog
{
    Q_OBJECT

public:
    explicit DOEGCdlg(QWidget *parent = 0);
    ~DOEGCdlg();

private slots:
    void on_create_clicked();

    void on_browse_clicked();

private:
    Ui::DOEGCdlg *ui;
};

#endif // DOEGCDLG_H
