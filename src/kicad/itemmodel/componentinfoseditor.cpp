#include "componentinfoseditor.h"

#include <QFormLayout>

ComponentInfosEditor::ComponentInfosEditor(QWidget *parent)
    : QWidget(parent)
{
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
        return;
    }

    _nameEdit->setEnabled(true);
    _nameEdit->setText(component->name());

    _packageCombo->setEnabled(true);
    _packageCombo->clear();
    _packageCombo->addItems(component->footPrints());
}

void ComponentInfosEditor::createWidgets()
{
    QFormLayout *layout = new QFormLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    _nameEdit = new QLineEdit();
    layout->addRow(tr("Name"), _nameEdit);

    _packageCombo = new QComboBox();
    _packageCombo->setEditable(true);
    layout->addRow(tr("Package"), _packageCombo);

    setLayout(layout);
}
