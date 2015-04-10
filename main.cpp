
#include <QApplication>
#include "fmprocedure.h"
#include "fmgishandler.h"
#include "optique.h"
#include "mainwindow.h"

#include "ui/mainfm.h"

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);
    QApplication b(argc, argv);

    MainFM w;
    w.show();

    return b.exec();
}
