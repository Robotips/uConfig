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

#include "componentwidget.h"

#include <QAction>
#include <QLabel>
#include <QLayout>

ComponentWidget::ComponentWidget(QWidget *parent)
    : QWidget(parent)
{
    Q_INIT_RESOURCE(imgviewer);
    _component = nullptr;
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

    if (_component != nullptr)
    {
        char c = 'A';
        for (int i = 0; i < _component->unitCount(); i++, c++)
        {
            _comboUnit->addItem(QString("Unit ") + c);
        }
        _comboUnit->setEnabled(_component->unitCount() != 1);
    }

    _viewer->setComponent(_component);
}

void ComponentWidget::createWidgets()
{
    _viewer = new ComponentViewer();

    QLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    QLayout *_toolsLayout = new QHBoxLayout();

    _toolsLayout->addItem(new QSpacerItem(5, 5));
    _gridGroup = new QActionGroup(this);
    _gridGroup->setExclusive(true);

    _actionNoGrid = new QAction(this);
    _actionNoGrid->setText(tr("Hide grid"));
    _actionNoGrid->setCheckable(true);
    _actionNoGrid->setIcon(QIcon(":/iconsviewer/img/view-nogrid.png"));
    _gridGroup->addAction(_actionNoGrid);
    QToolButton *toolNoGrid = new QToolButton();
    toolNoGrid->setMinimumSize(30, 30);
    toolNoGrid->setDefaultAction(_actionNoGrid);
    _toolsLayout->addWidget(toolNoGrid);

    _actionGrid = new QAction(this);
    _actionGrid->setText(tr("Show grid"));
    _actionGrid->setCheckable(true);
    _actionGrid->setChecked(true);
    _actionGrid->setIcon(QIcon(":/iconsviewer/img/view-grid.png"));
    _gridGroup->addAction(_actionGrid);
    QToolButton *toolGrid = new QToolButton();
    toolGrid->setMinimumSize(30, 30);
    toolGrid->setDefaultAction(_actionGrid);
    _toolsLayout->addWidget(toolGrid);

    _actionGridFront = new QAction(this);
    _actionGridFront->setText(tr("Show grid front"));
    _actionGridFront->setCheckable(true);
    _actionGridFront->setIcon(QIcon(":/iconsviewer/img/view-grid-front.png"));
    _gridGroup->addAction(_actionGridFront);
    QToolButton *toolGridFront = new QToolButton();
    toolGridFront->setMinimumSize(30, 30);
    toolGridFront->setDefaultAction(_actionGridFront);
    _toolsLayout->addWidget(toolGridFront);
    connect(_gridGroup, &QActionGroup::triggered, this, &ComponentWidget::setGridVisible);

    _toolsLayout->addItem(new QSpacerItem(2, 2));
    _actionElecType = new QAction(this);
    _actionElecType->setText(tr("Toggle electrical type view"));
    _actionElecType->setCheckable(true);
    _actionElecType->setChecked(true);
    _actionElecType->setIcon(QIcon(":/iconsviewer/img/view-electype.png"));
    connect(_actionElecType, &QAction::toggled, _viewer, &ComponentViewer::setElecTypeVisible);
    QToolButton *toolElecType = new QToolButton();
    toolElecType->setMinimumSize(30, 30);
    toolElecType->setDefaultAction(_actionElecType);
    _toolsLayout->addWidget(toolElecType);

    _toolsLayout->addItem(new QSpacerItem(5, 5));
    _toolsLayout->addWidget(new QLabel("Unit: "));
    _comboUnit = new QComboBox();
    _toolsLayout->addWidget(_comboUnit);
    connect(_comboUnit, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ComponentWidget::setUnit);

    _toolsLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));

    layout->addItem(_toolsLayout);
    layout->addWidget(_viewer);

    setLayout(layout);
}

QAction *ComponentWidget::actionElecType() const
{
    return _actionElecType;
}

QAction *ComponentWidget::actionNoGrid() const
{
    return _actionNoGrid;
}

QAction *ComponentWidget::actionGrid() const
{
    return _actionGrid;
}

QAction *ComponentWidget::actionGridFront() const
{
    return _actionGridFront;
}

ComponentViewer *ComponentWidget::viewer() const
{
    return _viewer;
}

void ComponentWidget::setUnit(int unit)
{
    _viewer->setComponent(_component, unit + 1);
}

void ComponentWidget::setGridVisible(QAction *action)
{
    if (action == _actionNoGrid)
    {
        _viewer->setGridVisible(false);
        _viewer->setGridFront(false);
    }
    else if (action == _actionGrid)
    {
        _viewer->setGridVisible(true);
        _viewer->setGridFront(false);
    }
    else if (action == _actionGridFront)
    {
        _viewer->setGridVisible(true);
        _viewer->setGridFront(true);
    }
}
