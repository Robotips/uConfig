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

    _statusLabel = new QLabel(tr("processing..."));
    layout->addWidget(_statusLabel);

    _componentTreeView = new ComponentLibTreeView();
    layout->addWidget(_componentTreeView);

    setLayout(layout);
}

int ComponentsPage::nextId() const
{
    return PinListImporter::PageResults;
}

void ComponentsPage::initializePage()
{
    QString filepdf = field("file").toString();
    QFileInfo info(filepdf);
    QString fileName = info.fileName();

    PinListImporter::ImportType type = static_cast<PinListImporter*>(wizard())->type();
    if (type == PinListImporter::Kicad)
    {
        Lib lib;
        lib.readFrom(filepdf);
        foreach (Component *component, lib.components())
        {
            _componentTreeView->addComponent(component);
        }
    }
}

bool ComponentsPage::isComplete() const
{
    return true;
}
