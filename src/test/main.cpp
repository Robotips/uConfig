#include <QDebug>

#include "../datasheet_extractor/datasheet.h"

int main()
{
    Datasheet datasheet;
    qDebug()<<datasheet.open("/home/seb/Seafile/projects/DataSheets/Microchip/PIC32/PIC32MM_GPM_revC.pdf");
    datasheet.analyse();
    foreach(DatasheetPackage *package, datasheet.packages())
        qDebug()<<package->name;
}
