#include "componentspage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileInfo>

#include "pinlistimporter.h"

#include "../kicad/model/lib.h"

ComponentsPage::ComponentsPage()
    : QWizardPage(0)
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
    QList<Component *> &components = static_cast<PinListImporter*>(wizard())->components();
    PinListImporter::ImportType type = static_cast<PinListImporter*>(wizard())->type();
    _lib = new Lib();
    if (type == PinListImporter::Kicad)
    {
        QString file = field("file").toString();
        static_cast<PinListImporter*>(wizard())->setFilePath(file);
        components.clear();
        if (!_lib->readFrom(file))
            _statusLabel->setText(tr("Cannot parse library file"));
    }
    else
    {
        foreach (Component *component, components)
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
    QList<Component *> &components = static_cast<PinListImporter*>(wizard())->components();
    components.clear();
    foreach (Component *component, _componentTreeView->selectedComponents())
    {
        components.append(component);
        _lib->takeComponent(component);
    }
    _componentTreeView->setLib(Q_NULLPTR);
    delete _lib;

    return true;
}
