#include "pinlisteditor.h"

#include <QVBoxLayout>
#include <QLabel>

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

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(new QLabel(tr("Pin name filter: ")));
    _nameFilterEdit = new QLineEdit();
    headerLayout->addWidget(_nameFilterEdit);
    layout->addItem(headerLayout);

    _componentPinsTableView = new ComponentPinsTableView();
    layout->addWidget(_componentPinsTableView);

    connect(_nameFilterEdit, &QLineEdit::textChanged, _componentPinsTableView, &ComponentPinsTableView::setPinFilter);

    setLayout(layout);
}
