#include "datasheetresultspage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileInfo>

#include "pinlistimporter.h"

DatasheetResultsPage::DatasheetResultsPage() : QWizardPage(0)
{
    QVBoxLayout *layout = new QVBoxLayout;
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
    QString filepdf = field("filepdf").toString();
    QFileInfo info(filepdf);
    QString fileName = info.fileName();

    _datasheet.setDebugEnabled(true);
    _datasheet.open(filepdf);
    setTitle(QString("Extracted packages in %1").arg(fileName));
    _datasheet.analyse();

    QString resText = QString("Extracted packages in %1:").arg(fileName);
    foreach (DatasheetPackage *pack, _datasheet.packages())
    {
        resText.append(QString("\n - %1 with %2 pins")
                           .arg(pack->name)
                           .arg(pack->pins.count()));
    }
    _resultLabel->setText(resText);
}

Datasheet *DatasheetResultsPage::datasheet()
{
    return &_datasheet;
}

bool DatasheetResultsPage::isComplete() const
{
    return true;
}
