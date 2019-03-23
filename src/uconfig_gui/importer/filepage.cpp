#include "filepage.h"

#include <QLabel>
#include <QDir>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVariant>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QFileInfo>

#include "pinlistimporter.h"

FilePage::FilePage() :
    QWizardPage(0)
{
    _complete = false;

    setAcceptDrops(true);

    QLabel *label = new QLabel("File:");
    label->setWordWrap(true);

    _fileEdit = new QLineEdit;
    connect(_fileEdit, SIGNAL(textChanged(QString)), this, SLOT(checkEntry(QString)));

    QToolButton *fileExplore = new QToolButton;
    fileExplore->setText("...");
    connect(fileExplore, SIGNAL(clicked()), this, SLOT(fileExplore()));

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
    switch (static_cast<PinListImporter*>(wizard())->type())
    {
    case PinListImporter::Kicad:
        return PinListImporter::PageComponents;
    case PinListImporter::CSV:
        return PinListImporter::PagePDFProcess;
    case PinListImporter::PDF:
        return PinListImporter::PagePDFFile;
    case PinListImporter::Table:
        return PinListImporter::PagePDFProcess;
    }
    return 0;
}

void FilePage::initializePage()
{
    switch (static_cast<PinListImporter*>(wizard())->type())
    {
    case PinListImporter::Kicad:
        _fileTitle = "Kicad lib";
        _suffixes << "lib";
        break;
    case PinListImporter::CSV:
        _fileTitle = "coma separator";
        _suffixes << "csv";
        break;
    case PinListImporter::PDF:
        _fileTitle = "pdf datasheet";
        _suffixes << "pdf";
        break;
    case PinListImporter::Table:
        _fileTitle = "excel sheet";
        _suffixes << "xls" << "xlsx";
        break;
    }

    setTitle(QString("Choose a %1 file").arg(_fileTitle));

    if (!_fileEdit->text().isEmpty())
        checkEntry(_fileEdit->text());
}

void FilePage::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->accept();
}

void FilePage::dropEvent(QDropEvent *event)
{
    setFile(event->mimeData()->urls().first().toLocalFile());
}

QMap<PinListImporter::ImportType, QString> FilePage::_importTypeSettingsKeymap = {
    {PinListImporter::Kicad, "Kicad"},
    {PinListImporter::PDF, "PDF"},
    {PinListImporter::Table, "Table"},
    {PinListImporter::CSV, "CSV"},
};

void FilePage::fileExplore()
{
    QString lastPath;
    if (_importTypeSettingsKeymap.contains(static_cast<PinListImporter*>(wizard())->type())) {
        _settings.beginGroup("FilePage");
        _settings.beginGroup(_importTypeSettingsKeymap.value(static_cast<PinListImporter*>(wizard())->type()));
        lastPath = _settings.value("lastPath", QDir::homePath()).toString();
        _settings.endGroup();
        _settings.endGroup();
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QString("Choose a %1 file").arg(_fileTitle),
                                                    lastPath,
                                                    QString("%1 (%2)").arg(_fileTitle).arg("*."+_suffixes.join(" *.")));
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
    if (_importTypeSettingsKeymap.contains(static_cast<PinListImporter*>(wizard())->type())) {
        _settings.beginGroup("FilePage");
        _settings.beginGroup(_importTypeSettingsKeymap.value(static_cast<PinListImporter*>(wizard())->type()));
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
