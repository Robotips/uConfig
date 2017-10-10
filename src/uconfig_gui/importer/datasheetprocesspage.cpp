#include "datasheetprocesspage.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileInfo>

#include "pinlistimporter.h"

DatasheetProcessPage::DatasheetProcessPage()
    : QWizardPage(0)
{
    QVBoxLayout *layout = new QVBoxLayout;

    _statusLabel = new QLabel("processing...");
    layout->addWidget(_statusLabel);

    _progressBar = new QProgressBar();
    layout->addWidget(_progressBar);

    _progressLabel = new QLabel();
    layout->addWidget(_progressLabel);

    _logger = new QPlainTextEdit();
    _logger->setReadOnly(true);
    layout->addWidget(_logger);
    setLayout(layout);

    _thread = new DataSheetThread(&_datasheet);
    _complete = false;
}

int DatasheetProcessPage::nextId() const
{
    return PinListImporter::PagePDFResults;
}

void DatasheetProcessPage::initializePage()
{
    QString filepdf = field("file").toString();
    QFileInfo info(filepdf);
    QString fileName = info.fileName();

    _datasheet.setDebugEnabled(true);
    _datasheet.open(filepdf);
    setTitle(QString("Extracting packages in %1").arg(fileName.right(30)));

    _progressBar->setMaximum(_datasheet.pageCount());
    connect(&_datasheet, &Datasheet::pageChanged, this, &DatasheetProcessPage::changePage);
    connect(&_datasheet, &Datasheet::log, this, &DatasheetProcessPage::addLog);

    _thread->start();
    connect(_thread, &QThread::finished, this, &DatasheetProcessPage::finish);
}

Datasheet *DatasheetProcessPage::datasheet()
{
    return &_datasheet;
}

void DatasheetProcessPage::finish()
{
    _complete = true;
    _statusLabel->setText("terminated");
    emit completeChanged();
}

void DatasheetProcessPage::changePage(int page)
{
    _progressBar->setValue(page);
    _progressLabel->setText(QString("page %1 / %2").arg(page).arg(_progressBar->maximum()));
}

void DatasheetProcessPage::addLog(QString str)
{
    _logger->appendPlainText(str);
}

bool DatasheetProcessPage::isComplete() const
{
    return _complete;
}
