#include "datasheetresultspage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileInfo>

#include "pinlistimporter.h"

DatasheetResultsPage::DatasheetResultsPage(DataSheetThread *datasheetThread, QList<Component *> &components)
    : QWizardPage(0), _components(components)
{
    QVBoxLayout *layout = new QVBoxLayout;
    _datasheetThread = datasheetThread;
    _resultLabel = new QLabel();
    layout->addWidget(_resultLabel);
    layout->addSpacerItem(new QSpacerItem(10, 30,
                                          QSizePolicy::Expanding,
                                          QSizePolicy::Expanding));
    setLayout(layout);
}

int DatasheetResultsPage::nextId() const
{
    return -1;
}

void DatasheetResultsPage::initializePage()
{
    QString filepdf = field("file").toString();
    QFileInfo info(filepdf);
    QString fileName = info.fileName();
    setTitle(QString("Extracted packages in %1").arg(fileName.right(30)));

    QString resText = QString("Packages found:");
    foreach (DatasheetPackage *pack, _datasheetThread->datasheet()->packages())
    {
        resText.append(QString("\n - %1 with %2 pins")
                           .arg(pack->name)
                           .arg(pack->pins.count()));
    }
    _resultLabel->setText(resText);
}

bool DatasheetResultsPage::validatePage()
{
    qDeleteAll(_components);
    _components.clear();
    foreach (Component *component, _datasheetThread->datasheet()->components())
    {
        _components.append(component);
    }
    return true;
}

bool DatasheetResultsPage::isComplete() const
{
    return true;
}
