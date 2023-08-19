/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QScreen>

#include <pdf_extract/datasheet.h>

#include <kicad/model/lib.h>
#include <kicad/viewer/componentviewer.h>

#include <kicad/pinruler/pinruler.h>
#include <kicad/pinruler/rulesparser.h>

#include <pdf_extract/model/pdfdatasheet.h>
#include <pdf_extract/pdfdebugwidget/pdfdebugwidget.h>

void widthCompute();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MM_GPM_revC.pdf");
    // qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MM_GPL_revB.pdf");
    // qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MZ EF.pdf");
    // qDebug()<<datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MX1xx-2xx-5xx-64-100pins_revD.pdf");
    /*qDebug()<<datasheet.open("../../../projects/DataSheets/ST/STM32M7.pdf");
    qint64 d = QDateTime::currentMSecsSinceEpoch();
    datasheet.analyse(48);
    for (DatasheetPackage *package : datasheet.packages())
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

    /*    Datasheet datasheet;
        datasheet.setDebugEnabled();
        datasheet.open("../src/autotest/ATmega328P_pins.pdf");
        datasheet.analyse();
        if (datasheet.packages().empty())
            return 0;

        Component *component = datasheet.packages()[0]->toComponent();
        component->reorganizeToPackageStyle();

        ComponentViewer cviewer;
            cviewer.setComponent(component);
            cviewer.show();
            cviewer.resize(QApplication::primaryScreen()->size()*.7);

        return a.exec();*/

    /*RulesSet ruleSet;
    RulesParser parser("../rules/atmel.kss");
    parser.parse(&ruleSet);

    PinRuler ruler(&ruleSet);

    Lib lib;
    //lib.readFrom("/home/seb/Seafile/my_lib_rt/kicad/kicad-symbols/MCU_Microchip_PIC16.lib");
    lib.readFrom("C:/Program Files/KiCad/share/kicad/library/MCU_ST_STM32.lib");
    //lib.readFrom("/home/seb/Seafile/UniSwarm/pcb/motors4/PIC32MK_revD.lib");

    Component *component = lib.components()[0];
    for (int i=0; i<lib.componentsCount(); i++)
        if (lib.component(i)->name() == "STM32F413ZHTx")
            component = lib.components()[i];
    ruler.organize(component);*/

    // lib.saveTo("/home/seb/Seafile/UniSwarm/pcb/PIC32MK_revD.lib");

    /*ComponentScene scene(0,0,0,0);
    scene.setComponent(component);
    scene.saveAsPdf("test.pdf");
    scene.saveAsImage("test.png");
    lib.saveTo("C:/Users/sebas/Desktop/stm8b.lib");*/

    /*ComponentViewer viewer;
    viewer.setComponent(component);
    viewer.show();
    viewer.resize(QApplication::primaryScreen()->size()*.7);*/

    PDFDatasheet pdf("../src/autotest/ATmega328P_pins.pdf");
    //    PDFDatasheet pdf("../src/autotest/ATmega328P_pins.pdf");
    // PDFDatasheet pdf("C:/Users/seb/Seafile/UniSwarm/DataSheets/Microchip/PIC16b/dsPIC33EP/PIC24-dsPIC33-EPxxxGP-MC-20x-50x_revH.pdf");

    PdfDebugWidget viewer(&pdf);
    viewer.showPage(0);
    viewer.show();

    return QApplication::exec();
}
