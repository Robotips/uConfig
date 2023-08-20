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

#include "filepage.h"

#include <QDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMimeData>
#include <QToolButton>
#include <QUrl>
#include <QVariant>

#include "pinlistimporter.h"

FilePage::FilePage()
    : QWizardPage(nullptr)
{
    _complete = false;

    setAcceptDrops(true);

    QLabel *label = new QLabel("File:");
    label->setWordWrap(true);

    _fileEdit = new QLineEdit;
    connect(_fileEdit, &QLineEdit::textChanged, this, &FilePage::checkEntry);

    QToolButton *fileExplore = new QToolButton;
    fileExplore->setText("...");
    connect(fileExplore, &QToolButton::clicked, this, &FilePage::fileExplore);

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(label);
    hlayout->addWidget(_fileEdit);
    hlayout->addWidget(fileExplore);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hlayout);
    layout->addSpacerItem(new QSpacerItem(10, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    setLayout(layout);

    registerField("file", _fileEdit);
}

int FilePage::nextId() const
{
    switch (dynamic_cast<PinListImporter *>(wizard())->type())
    {
        case PinListImporter::Kicad:
            return PinListImporter::PageComponents;
        case PinListImporter::CSV:
            return PinListImporter::PageComponents;
        case PinListImporter::PDF:
            return PinListImporter::PagePDFFile;
            /*
            case PinListImporter::Table:
                return PinListImporter::PagePDFProcess;
            */
    }
    return 0;
}

void FilePage::initializePage()
{
    switch (dynamic_cast<PinListImporter *>(wizard())->type())
    {
        case PinListImporter::Kicad:
            _fileTitle = "Kicad lib (.lib)";
            _suffixes << "lib";
            break;
        case PinListImporter::CSV:
            _fileTitle = "coma separator (.csv|.txt)";
            _suffixes << "csv"
                      << "txt";
            break;
        case PinListImporter::PDF:
            _fileTitle = "pdf datasheet (.pdf)";
            _suffixes << "pdf";
            break;
            /*
            case PinListImporter::Table:
                _fileTitle = "excel sheet";
                _suffixes << "xls"
                          << "xlsx";
                break;
            */
    }

    setTitle(QString("Choose a %1 file").arg(_fileTitle));

    if (!_fileEdit->text().isEmpty())
    {
        checkEntry(_fileEdit->text());
    }
}

void FilePage::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
    }
}

void FilePage::dropEvent(QDropEvent *event)
{
    setFile(event->mimeData()->urls().first().toLocalFile());
}

QMap<PinListImporter::ImportType, QString> FilePage::_importTypeSettingsKeymap = {
    {PinListImporter::Kicad, "Kicad"},
    {PinListImporter::PDF, "PDF"},
    //{PinListImporter::Table, "Table"},
    {PinListImporter::CSV, "CSV"},
};

void FilePage::fileExplore()
{
    QString lastPath;
    if (_importTypeSettingsKeymap.contains(dynamic_cast<PinListImporter *>(wizard())->type()))
    {
        _settings.beginGroup("FilePage");
        _settings.beginGroup(_importTypeSettingsKeymap.value(dynamic_cast<PinListImporter *>(wizard())->type()));
        lastPath = _settings.value("lastPath", QDir::homePath()).toString();
        _settings.endGroup();
        _settings.endGroup();
    }

    QString fileName = QFileDialog::getOpenFileName(
        this, QString("Choose a %1 file").arg(_fileTitle), lastPath, QString("%1 (%2)").arg(_fileTitle).arg("*." + _suffixes.join(" *.")));
    if (!fileName.isEmpty())
    {
        setFile(fileName);
    }
}

void FilePage::checkEntry(const QString &text)
{
    QFileInfo info(text);
    QPalette palette = _fileEdit->palette();
    if (!_suffixes.contains(info.suffix(), Qt::CaseInsensitive) || !info.exists())
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
    _fileEdit->setPalette(palette);
}

void FilePage::setFile(const QString &file)
{
    _fileEdit->setText(file);
    if (_importTypeSettingsKeymap.contains(dynamic_cast<PinListImporter *>(wizard())->type()))
    {
        _settings.beginGroup("FilePage");
        _settings.beginGroup(_importTypeSettingsKeymap.value(dynamic_cast<PinListImporter *>(wizard())->type()));
        _settings.setValue("lastPath", file);
        _settings.endGroup();
        _settings.endGroup();
    }
}

bool FilePage::isComplete() const
{
    return _complete;
}

QString FilePage::file() const
{
    return _file;
}
