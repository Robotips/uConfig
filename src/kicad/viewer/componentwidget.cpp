/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
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

#include "componentwidget.h"

#include <QAction>
#include <QLabel>
#include <QLayout>

ComponentWidget::ComponentWidget(QWidget *parent)
    : QWidget(parent)
{
    Q_INIT_RESOURCE(imgviewer);
    _component = Q_NULLPTR;
    createWidgets();
}

Component *ComponentWidget::component() const
{
    return _component;
}

void ComponentWidget::setComponent(Component *component)
{
    _component = component;
    _comboUnit->clear();

    if (_component)
    {
        char c = 'A';
        for (int i=0; i<_component->unitCount(); i++,c++)
            _comboUnit->addItem(QString("Unit ") + c);
        _comboUnit->setEnabled(_component->unitCount() != 1);
    }

    _viewer->setComponent(_component);
}

void ComponentWidget::createWidgets()
{
    _viewer = new ComponentViewer();

    QLayout *_layout = new QVBoxLayout();
    _layout->setMargin(0);
    QLayout *_toolsLayout = new QHBoxLayout();

    _toolsLayout->addItem(new QSpacerItem(5, 5));
    _ationGrid = new QAction(this);
    _ationGrid->setText(tr("Toggle grid view"));
    _ationGrid->setCheckable(true);
    _ationGrid->setChecked(true);
    _ationGrid->setIcon(QIcon(":/iconsviewer/img/view-grid.png"));
    connect(_ationGrid, &QAction::toggled, _viewer, &ComponentViewer::setGridVisible);
    QToolButton *toolGrid = new QToolButton();
    toolGrid->setMinimumSize(30, 30);
    toolGrid->setDefaultAction(_ationGrid);
    _toolsLayout->addWidget(toolGrid);

    _toolsLayout->addItem(new QSpacerItem(2, 2));
    _ationElecType = new QAction(this);
    _ationElecType->setText(tr("Toggle electrical type view"));
    _ationElecType->setCheckable(true);
    _ationElecType->setChecked(true);
    _ationElecType->setIcon(QIcon(":/iconsviewer/img/view-electype.png"));
    connect(_ationElecType, &QAction::toggled, _viewer, &ComponentViewer::setElecTypeVisible);
    QToolButton *toolElecType = new QToolButton();
    toolElecType->setMinimumSize(30, 30);
    toolElecType->setDefaultAction(_ationElecType);
    _toolsLayout->addWidget(toolElecType);

    _toolsLayout->addItem(new QSpacerItem(5, 5));
    _toolsLayout->addWidget(new QLabel("Unit: "));
    _comboUnit = new QComboBox();
    _toolsLayout->addWidget(_comboUnit);
    connect(_comboUnit, SIGNAL(currentIndexChanged(int)), this, SLOT(setUnit(int)));

    _toolsLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));

    _layout->addItem(_toolsLayout);
    _layout->addWidget(_viewer);

    setLayout(_layout);
}

QAction *ComponentWidget::ationElecType() const
{
    return _ationElecType;
}

QAction *ComponentWidget::ationGrid() const
{
    return _ationGrid;
}

ComponentViewer *ComponentWidget::viewer() const
{
    return _viewer;
}

void ComponentWidget::setUnit(int unit)
{
    _viewer->setComponent(_component, unit+1);
}
