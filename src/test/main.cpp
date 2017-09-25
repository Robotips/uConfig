#include <QDebug>
#include <QApplication>
#include <QDateTime>

#include "../pdf_extract/datasheet.h"
#include "../kicad/viewer/componentviewer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Datasheet datasheet;
    qDebug()<<datasheet.open("/home/seb/Seafile/projects/DataSheets/Microchip/PIC32/PIC32MM_GPM_revC.pdf");

    qint64 d = QDateTime::currentMSecsSinceEpoch();
    datasheet.analyse();
    foreach(DatasheetPackage *package, datasheet.packages())
        qDebug()<<package->name;
    qDebug()<<QDateTime::currentMSecsSinceEpoch() - d;
    exit(0);

    //ComponentViewer viewer;
    //viewer.setComponent(datasheet.packages()[0]->toComponent());
    //viewer.show();

    return a.exec();
}
