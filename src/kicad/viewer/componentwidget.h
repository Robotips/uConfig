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

#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

#include <QActionGroup>
#include <QComboBox>
#include <QToolButton>
#include <QWidget>
#include <QtCore/qglobal.h>

#include "componentviewer.h"

class KICAD_EXPORT ComponentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComponentWidget(QWidget *parent = nullptr);

    Component *component() const;
    void setComponent(Component *component);

    ComponentViewer *viewer() const;

    QAction *actionNoGrid() const;
    QAction *actionGrid() const;
    QAction *actionGridFront() const;
    QAction *actionElecType() const;

public slots:
    void setUnit(int unit);

protected slots:
    void setGridVisible(QAction *action);

protected:
    void createWidgets();

    Component *_component;
    ComponentViewer *_viewer;
    QActionGroup *_gridGroup;
    QAction *_actionNoGrid;
    QAction *_actionGrid;
    QAction *_actionGridFront;
    QAction *_actionElecType;
    QComboBox *_comboUnit;
};

#endif  // COMPONENTWIDGET_H
