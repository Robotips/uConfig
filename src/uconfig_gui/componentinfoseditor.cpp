#include "componentinfoseditor.h"

#include <QFormLayout>

ComponentInfosEditor::ComponentInfosEditor(UConfigProject *project)
{
    _project = project;
    createWidgets();
    setComponent(Q_NULLPTR);
}

Component *ComponentInfosEditor::component() const
{
    return _component;
}

void ComponentInfosEditor::setComponent(Component *component)
{
    _component = component;
    if (_component == Q_NULLPTR)
    {
        _nameEdit->setEnabled(false);
        _packageCombo->setEnabled(false);
        _referenceEdit->setEnabled(false);
        return;
    }

    _nameEdit->setEnabled(true);
    _nameEdit->setText(component->name());

    _packageCombo->setEnabled(true);
    _packageCombo->clear();
    _packageCombo->addItems(component->footPrints());

    _referenceEdit->setEnabled(true);
    _referenceEdit->setText(component->prefix());
}

void ComponentInfosEditor::setComponentName()
{
    _project->setComponentInfo(UConfigProject::ComponentNameInfo, _nameEdit->text());
}

void ComponentInfosEditor::setComponentReference()
{
    _project->setComponentInfo(UConfigProject::ComponentReferenceInfo, _referenceEdit->text());
}

void ComponentInfosEditor::createWidgets()
{
    QFormLayout *layout = new QFormLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    _nameEdit = new QLineEdit();
    connect(_nameEdit, &QLineEdit::editingFinished, this, &ComponentInfosEditor::setComponentName);
    layout->addRow(tr("Name"), _nameEdit);

    _packageCombo = new QComboBox();
    _packageCombo->setEditable(true);
    layout->addRow(tr("Package"), _packageCombo);

    _referenceEdit = new QLineEdit();
    connect(_referenceEdit, &QLineEdit::editingFinished, this, &ComponentInfosEditor::setComponentReference);
    layout->addRow(tr("Reference"), _referenceEdit);

    setLayout(layout);
}
