/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include "datasheetprocesspage.h"

#include <QDebug>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>

#include "pinlistimporter.h"

DatasheetProcessPage::DatasheetProcessPage()
    : QWizardPage(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout;

    _statusLabel = new QLabel(tr("processing..."));
    layout->addWidget(_statusLabel);

    _progressBar = new QProgressBar();
    layout->addWidget(_progressBar);

    _progressLabel = new QLabel();
    layout->addWidget(_progressLabel);

    _logger = new QPlainTextEdit();
    _logger->setReadOnly(true);
    layout->addWidget(_logger);
    setLayout(layout);

    _datasheet = new Datasheet();
    _thread = new DataSheetThread(_datasheet);
    _complete = false;

    connect(_datasheet, &Datasheet::pageChanged, this, &DatasheetProcessPage::changePage);
    connect(_datasheet, &Datasheet::log, this, &DatasheetProcessPage::addLog);
}

int DatasheetProcessPage::nextId() const
{
    return PinListImporter::PageComponents;
}

void DatasheetProcessPage::initializePage()
{
    QString filepdf = field("file").toString();
    dynamic_cast<PinListImporter *>(wizard())->setFilePath(filepdf);
    QFileInfo info(filepdf);
    QString fileName = info.fileName();

    _datasheet->setDebugEnabled(true);
    setTitle(tr("Extracting packages in %1...").arg(fileName.right(30)));

    int _pageStart;
    int _pageCount;
    if (_thread->pageBegin() == -1 && _thread->pageEnd() == -1)  // whole document
    {
        _pageStart = 0;
        _pageCount = _datasheet->pageCount();
    }
    else if (_thread->pageEnd() == -1)
    {
        _pageStart = _thread->pageBegin();
        _pageCount = 1;
    }
    else
    {
        _pageStart = _thread->pageBegin();
        _pageCount = _thread->pageEnd() - _thread->pageBegin();
    }
    _progressBar->setMinimum(_pageStart + 1);
    _progressBar->setMaximum(_pageStart + _pageCount + 1);

    _logger->clear();
    _datasheet->clean();
    _thread->start();
    connect(_thread, &QThread::finished, this, &DatasheetProcessPage::finish);
}

DataSheetThread *DatasheetProcessPage::datasheetThread()
{
    return _thread;
}

void DatasheetProcessPage::finish()
{
    _complete = true;
    _statusLabel->setText(tr("terminated"));
    changePage(_progressBar->maximum());
    emit completeChanged();

    QList<Component *> &components = dynamic_cast<PinListImporter *>(wizard())->components();

    qDeleteAll(components);
    components.clear();
    for (Component *component : _thread->datasheet()->components())
    {
        components.append(component);
    }
}

void DatasheetProcessPage::changePage(int page)
{
    _progressBar->setValue(page);
    _progressLabel->setText(tr("page %1 / %2").arg(page).arg(_progressBar->maximum()));
}

void DatasheetProcessPage::addLog(const QString &str)
{
    _logger->appendPlainText(str);
}

bool DatasheetProcessPage::isComplete() const
{
    return _complete;
}
