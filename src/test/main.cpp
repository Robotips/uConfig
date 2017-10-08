#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QScreen>

#include "../pdf_extract/datasheet.h"

#include "../kicad/viewer/componentviewer.h"
#include "../kicad/pinclassifier.h"

#include "../kicad/pinruler/rulesparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Datasheet datasheet;
    //qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MM_GPM_revC.pdf");
    //qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MM_GPL_revB.pdf");
    //qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MZ EF.pdf");
    //qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MX1xx-2xx-5xx-64-100pins_revD.pdf");
    /*qDebug()<<datasheet.open("../../../projects/DataSheets/ST/STM32M7.pdf");
    qint64 d = QDateTime::currentMSecsSinceEpoch();
    datasheet.analyse(48);
    foreach(DatasheetPackage *package, datasheet.packages())
        qDebug()<<package->name;
    qDebug()<<QDateTime::currentMSecsSinceEpoch() - d;

    ComponentViewer viewer;
    if (!datasheet.packages().empty())
    {
        Component *component = datasheet.packages()[0]->toComponent();
        component->reorganizeToPackageStyle();
        viewer.setComponent(component);
        viewer.show();
        viewer.resize(QApplication::primaryScreen()->size()*.7);
    }
    else
        return 0;*/

    RulesParser parser("../tst.rule");

    return 0;

    return a.exec();
}
