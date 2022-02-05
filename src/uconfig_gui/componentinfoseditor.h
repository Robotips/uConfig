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

#ifndef COMPONENTINFOSEDITOR_H
#define COMPONENTINFOSEDITOR_H

#include <QWidget>

#include <QLineEdit>
#include <QPlainTextEdit>

#include "project/uconfigproject.h"

class ComponentInfosEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ComponentInfosEditor(UConfigProject *project);

    Component *component() const;

signals:

public slots:
    void setComponent(Component *component);

protected slots:
    void setComponentName();
    void setComponentReference();
    void setComponentPackages();
    void setComponentAliases();

protected:
    UConfigProject *_project;
    Component *_component;

    void createWidgets();
    QLineEdit *_nameEdit;
    QPlainTextEdit *_packageEdit;
    QLineEdit *_referenceEdit;
    QPlainTextEdit *_aliasesEdit;
};

#endif  // COMPONENTINFOSEDITOR_H
