#include "componentspage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileInfo>

#include "pinlistimporter.h"

#include "../kicad/lib.h"

ComponentsPage::ComponentsPage()
    : QWizardPage(0)
{
    QVBoxLayout *layout = new QVBoxLayout;

    _statusLabel = new QLabel(tr("Choose component to import:"));
    layout->addWidget(_statusLabel);

    _componentTreeView = new ComponentLibTreeView();
    _componentTreeView->setSelectedMode(true);
    layout->addWidget(_componentTreeView);

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
