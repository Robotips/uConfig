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

#include "componentinfoseditor.h"

#include <QFormLayout>

ComponentInfosEditor::ComponentInfosEditor(UConfigProject *project)
{
    _project = project;
    createWidgets();
    setComponent(nullptr);
}

Component *ComponentInfosEditor::component() const
{
    return _component;
}

void ComponentInfosEditor::setComponent(Component *component)
{
    _component = component;

    disconnect(_packageEdit, &QPlainTextEdit::textChanged, this, &ComponentInfosEditor::setComponentAliases);
    disconnect(_aliasesEdit, &QPlainTextEdit::textChanged, this, &ComponentInfosEditor::setComponentAliases);

    if (_component == nullptr)
    {
        _nameEdit->setEnabled(false);
        _nameEdit->setText("");

        _packageEdit->setEnabled(false);
        _packageEdit->setPlainText("");

        _referenceEdit->setEnabled(false);
        _referenceEdit->setText("U");

        _aliasesEdit->setEnabled(false);
        _aliasesEdit->setPlainText("");
        return;
    }

    _nameEdit->setEnabled(true);
    _nameEdit->setText(component->name());

    _packageEdit->setEnabled(true);
    _packageEdit->setPlainText(component->footPrints().join('\n'));
    connect(_packageEdit, &QPlainTextEdit::textChanged, this, &ComponentInfosEditor::setComponentAliases);

    _referenceEdit->setEnabled(true);
    _referenceEdit->setText(component->prefix());

    _aliasesEdit->setEnabled(true);
    _aliasesEdit->setPlainText(component->aliases().join('\n'));
    connect(_aliasesEdit, &QPlainTextEdit::textChanged, this, &ComponentInfosEditor::setComponentAliases);
}

void ComponentInfosEditor::setComponentName()
{
    _project->setComponentInfo(UConfigProject::ComponentNameInfo, _nameEdit->text());
}

void ComponentInfosEditor::setComponentReference()
{
    _project->setComponentInfo(UConfigProject::ComponentReferenceInfo, _referenceEdit->text());
}

void ComponentInfosEditor::setComponentPackages()
{
    _project->setComponentInfo(UConfigProject::ComponentPackagesInfo, _packageEdit->toPlainText().split('\n', Qt::SkipEmptyParts));
}

void ComponentInfosEditor::setComponentAliases()
{
    _project->setComponentInfo(UConfigProject::ComponentAliasesInfo, _aliasesEdit->toPlainText().split('\n', Qt::SkipEmptyParts));
}

void ComponentInfosEditor::createWidgets()
{
    QFormLayout *layout = new QFormLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    _nameEdit = new QLineEdit();
    connect(_nameEdit, &QLineEdit::editingFinished, this, &ComponentInfosEditor::setComponentName);
    layout->addRow(tr("Name"), _nameEdit);

    _aliasesEdit = new QPlainTextEdit();
    _aliasesEdit->setMaximumHeight(100);
    layout->addRow(tr("Alias(es)"), _aliasesEdit);

    _referenceEdit = new QLineEdit();
    connect(_referenceEdit, &QLineEdit::editingFinished, this, &ComponentInfosEditor::setComponentReference);
    layout->addRow(tr("Reference"), _referenceEdit);

    _packageEdit = new QPlainTextEdit();
    _packageEdit->setMaximumHeight(100);
    layout->addRow(tr("Package(s)"), _packageEdit);

    layout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));

    setLayout(layout);
}
