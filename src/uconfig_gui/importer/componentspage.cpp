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

#include "componentspage.h"

#include <QDebug>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>

#include "pinlistimporter.h"

#include <kicad/model/lib.h>

ComponentsPage::ComponentsPage()
    : QWizardPage(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout;

    _statusLabel = new QLabel(tr("Choose component to import:"));
    layout->addWidget(_statusLabel);

    _checkAllBox = new QCheckBox("Select all");
    _checkAllBox->setChecked(true);
    layout->addWidget(_checkAllBox);

    _componentTreeView = new ComponentLibTreeView();
    _componentTreeView->setSelectedMode(true);
    layout->addWidget(_componentTreeView);
    connect(_checkAllBox, SIGNAL(clicked(bool)), _componentTreeView, SLOT(selectAll(bool)));

    setLayout(layout);
}

int ComponentsPage::nextId() const
{
    return PinListImporter::PageResults;
}

void ComponentsPage::initializePage()
{
    QList<Component *> &components = dynamic_cast<PinListImporter *>(wizard())->components();
    PinListImporter::ImportType type = dynamic_cast<PinListImporter *>(wizard())->type();
    _lib = new Lib();
    if (type == PinListImporter::Kicad)
    {
        QString file = field("file").toString();
        dynamic_cast<PinListImporter *>(wizard())->setFilePath(file);
        components.clear();
        if (!_lib->readFrom(file))
        {
            _statusLabel->setText(tr("Cannot parse library file"));
        }
    }
    else
    {
        for (Component *component : components)
        {
            _lib->addComponent(component);
        }
    }
    _componentTreeView->setLib(_lib);
    _componentTreeView->selectAll();
    _componentTreeView->resizeColumnToContents(0);
    _componentTreeView->resizeColumnToContents(1);
}

bool ComponentsPage::isComplete() const
{
    return true;
}

bool ComponentsPage::validatePage()
{
    QList<Component *> &components = dynamic_cast<PinListImporter *>(wizard())->components();
    components.clear();
    for (Component *component : _componentTreeView->selectedComponents())
    {
        components.append(component);
        _lib->takeComponent(component);
    }
    _componentTreeView->setLib(nullptr);
    delete _lib;

    return true;
}
