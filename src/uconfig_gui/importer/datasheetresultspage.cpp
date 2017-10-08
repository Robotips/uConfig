#include "datasheetresultspage.h"

#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVariant>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QFileInfo>
#include <QDebug>

#include "pinlistimporter.h"

DatasheetResultsPage::DatasheetResultsPage() :
    QWizardPage(0)
{
    _complete = false;

    setTitle(QString("Choose a %1 file").arg(_fileTitle));

    QLabel *label = new QLabel("File:");
    label->setWordWrap(true);

    fileEdit = new QLineEdit;
    connect(fileEdit, SIGNAL(textChanged(QString)), this, SLOT(checkEntry(QString)));

    QToolButton *fileExplore = new QToolButton;
    fileExplore->setText("...");
    connect(fileExplore, SIGNAL(clicked()), this, SLOT(fileExplore()));

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(label);
    hlayout->addWidget(fileEdit);
    hlayout->addWidget(fileExplore);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hlayout);
    layout->addSpacerItem(new QSpacerItem(10,30,QSizePolicy::Expanding,QSizePolicy::Expanding));
    setLayout(layout);
}

int DatasheetResultsPage::nextId() const
{
    return -1;
}

void DatasheetResultsPage::initializePage()
{
    _datasheet.open(field("filepdf").toString());
    _datasheet.analyse();
    foreach (DatasheetPackage *pack, _datasheet.packages())
    {
        qDebug()<<pack->name;
    }
}

Datasheet *DatasheetResultsPage::datasheet()
{
    return &_datasheet;
}

void DatasheetResultsPage::fileExplore()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString("Choose a %1 file").arg(_fileTitle), QString(), QString("%1 (%2)").arg(_fileTitle).arg("*."+_suffixes.join(" *.")));
    if(!fileName.isEmpty())
    {
        setFile(fileName);
    }
}

void DatasheetResultsPage::checkEntry(const QString &text)
{
    QFileInfo info(text);
    QPalette palette = fileEdit->palette();
    if (!_suffixes.contains(info.completeSuffix(), Qt::CaseInsensitive) || !info.exists())
    {
        _complete = false;
        emit completeChanged();
        palette.setColor(QPalette::Text, Qt::red);
    }
    else
    {
        _complete = true;
        emit completeChanged();
        palette.setColor(QPalette::Text, Qt::black);
    }
    fileEdit->setPalette(palette);
}

void DatasheetResultsPage::setFile(const QString &file)
{
    fileEdit->setText(file);
}

bool DatasheetResultsPage::isComplete() const
{
    return _complete;
}
