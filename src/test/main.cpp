#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QScreen>

#include "../pdf_extract/datasheet.h"

#include "../kicad/viewer/componentviewer.h"

#include "../kicad/pinruler/pinruler.h"
#include "../kicad/pinruler/rulesparser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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

    Datasheet datasheet;
    datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MM_GPM_revC.pdf");
    datasheet.analyse(3);
    if (datasheet.packages().empty())
        return 0;

    Component *component = datasheet.packages()[0]->toComponent();
    //component->reorganizeToPackageStyle();

    RulesSet ruleSet;
    RulesParser parser("../rules/tst.rule");
    parser.parse(&ruleSet);

    PinRuler ruler(&ruleSet);
    ruler.organize(component);

    ComponentViewer viewer;
    viewer.setComponent(component);
    viewer.show();
    viewer.resize(QApplication::primaryScreen()->size()*.7);

    return a.exec();
}
