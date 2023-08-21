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

#include "model/lib.h"

#include <QDebug>
#include <QFile>

void test_libkicad()
{
    QFile file("C:/Users/seb/Seafile/projects/SwarmTips/elec/4-top/b-fpga-vision/doc/max10/pinout.csv");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);

    Lib lib;
    // lib.readFrom("E:/doc/Seb/Mes document/Dropbox/Robot2015perso/KicadCreator/src/Ti.lib");
    Component *component = new Component();
    component->setName("Max10_ETQFP144");

    while (!stream.atEnd())
    {
        QString line = stream.readLine();
        QString name;
        QStringList items = line.split(";");

        int i = 0;
        for (const QString &item : items)
        {
            if (i > 0 && !item.isEmpty())
            {
                if (!name.isEmpty())
                {
                    name.append("/");
                }
                name.append(item);
            }
            i++;
        }

        if (items.count() > 1)
        {
            component->addPin(new Pin(name, items.at(0)));
        }
    }
    component->sort();

    QPoint gndpos(0, 0);
    QPoint vccpos(500, 0);
    QList<QPoint> points;

    for (int i = 0; i < 9; i++)
    {
        points.append(QPoint(1000 + 300 * i, 0));
    }

    for (auto pin : component->pins())
    {
        if (pin->name() == "GND")
        {
            pin->setPos(gndpos);
            pin->setAngle(180);
            gndpos += QPoint(0, 100);
        }
        else if (pin->name().startsWith("VCC"))
        {
            pin->setPos(vccpos);
            pin->setAngle(180);
            vccpos += QPoint(0, 100);
        }
        else
        {
            int bank = pin->name().at(0).toLatin1() - '0' - 1;
            if (bank >= 0 && bank < 8)
            {
                pin->setPos(points[bank]);
                pin->setAngle(180);
                points[bank] += QPoint(0, 100);
            }
        }
    }

    lib.addComponent(component);
    lib.saveTo("C:/Users/seb/Seafile/projects/maxcam/elec/max102.lib");
}
