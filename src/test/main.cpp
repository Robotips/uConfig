#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QScreen>

#include "../pdf_extract/datasheet.h"

#include "../kicad/viewer/componentviewer.h"
#include "../kicad/pinclassifier.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Datasheet datasheet;
    //qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MM_GPM_revC.pdf");
    //qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MM_GPL_revB.pdf");
    qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/out.pdf");

    qint64 d = QDateTime::currentMSecsSinceEpoch();
    datasheet.analyse(2);
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
        a.quit();

    return a.exec();
}
