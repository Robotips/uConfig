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
    app.setOrganizationName("Robotips");
    app.setOrganizationDomain("Robotips");
    app.setApplicationName("uConfig");

    UConfigProject project;
    UConfigMainWindow mainWindow(&project);
    project.setWindow(&mainWindow);
    mainWindow.show();
    if (app.arguments().size() > 1)
    {
        QString fileArg = app.arguments()[1];
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

    QFile f(":qdarkstyle/style.qss");
    if (f.exists())
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        app.setStyleSheet(ts.readAll());
        mainWindow.setStyleSheet("\
            QScrollArea {border: none;} \
            QComboBox::item:checked {height: 24px;} \
            QMenu::icon {margin: 1px;} \
            QMenu::icon:checked {margin: 0; background: #505F69; border: 1px inset #505F69; position: absolute; top: 1px; right: 1px; bottom: 1px; left: 1px;} \
            QMenu::item {padding: 4px 24px 4px 8px;} \
            QToolBar {border-bottom: none; border-radius: 4px;} \
            QSplitter::handle:horizontal {width: 2px;} \
            QSplitter::handle:vertical {height: 2px;} \
            QGroupBox {padding: 4px 0px 0px 0px;} \
        ");
    }

    return app.exec();
}
