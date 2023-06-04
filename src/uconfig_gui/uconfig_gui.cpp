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
#include <QDebug>

#include "project/uconfigproject.h"
#include "uconfigmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationName("Robotips");
    QApplication::setOrganizationDomain("Robotips");
    QApplication::setApplicationName("uConfig");

    UConfigProject project;
    UConfigMainWindow mainWindow(&project);
    project.setWindow(&mainWindow);
    mainWindow.show();
    if (QApplication::arguments().size() > 1)
    {
        QString fileArg = QApplication::arguments()[1];
        if (fileArg.endsWith(".lib", Qt::CaseInsensitive))
        {
            project.openLib(fileArg);
        }
        else
        {
            project.importComponents(fileArg);
        }
    }
    else
    {
        project.newLib();
    }

    return QApplication::exec();
}
