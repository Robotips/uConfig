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

    disconnect(_packageEdit, &QPlainTextEdit::textChanged, this, &ComponentInfosEditor::setComponentAliases);
    disconnect(_aliasesEdit, &QPlainTextEdit::textChanged, this, &ComponentInfosEditor::setComponentAliases);

    if (_component == Q_NULLPTR)
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
    _project->setComponentInfo(UConfigProject::ComponentPackagesInfo, _packageEdit->toPlainText().split('\n', QString::SkipEmptyParts));
}

void ComponentInfosEditor::setComponentAliases()
{
    _project->setComponentInfo(UConfigProject::ComponentAliasesInfo, _aliasesEdit->toPlainText().split('\n', QString::SkipEmptyParts));
}

void ComponentInfosEditor::createWidgets()
{
    QFormLayout *layout = new QFormLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    _nameEdit = new QLineEdit();
    connect(_nameEdit, &QLineEdit::editingFinished, this, &ComponentInfosEditor::setComponentName);
    layout->addRow(tr("Name"), _nameEdit);

    _packageEdit = new QPlainTextEdit();
    _packageEdit->setMaximumHeight(100);
    layout->addRow(tr("Package"), _packageEdit);

    _referenceEdit = new QLineEdit();
    connect(_referenceEdit, &QLineEdit::editingFinished, this, &ComponentInfosEditor::setComponentReference);
    layout->addRow(tr("Reference"), _referenceEdit);

    _aliasesEdit = new QPlainTextEdit();
    _aliasesEdit->setMaximumHeight(100);
    layout->addRow(tr("Aliases"), _aliasesEdit);

    setLayout(layout);
}
