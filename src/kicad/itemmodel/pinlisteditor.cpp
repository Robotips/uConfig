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

#include "pinlisteditor.h"

#include <QLabel>
#include <QVBoxLayout>

PinListEditor::PinListEditor(QWidget *parent)
    : QWidget(parent)
{
    createWidgets();
}

Component *PinListEditor::component() const
{
    return _componentPinsTableView->component();
}

void PinListEditor::setComponent(Component *component)
{
    _componentPinsTableView->setComponent(component);
}

ComponentPinsTableView *PinListEditor::tableView() const
{
    return _componentPinsTableView;
}

void PinListEditor::createWidgets()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 5, 0, 0);
    layout->setSpacing(5);

    _nameFilterEdit = new QLineEdit();
    _nameFilterEdit->setClearButtonEnabled(true);
    _nameFilterEdit->setPlaceholderText("Pin name filter...");
    layout->addWidget(_nameFilterEdit);

    _componentPinsTableView = new ComponentPinsTableView();
    layout->addWidget(_componentPinsTableView);

    connect(_nameFilterEdit, &QLineEdit::textChanged, _componentPinsTableView, &ComponentPinsTableView::setPinFilter);

    setLayout(layout);
}
