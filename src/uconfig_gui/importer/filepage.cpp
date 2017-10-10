#include "filepage.h"

#include <QLabel>
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

FilePage::FilePage(const int type) :
    QWizardPage(0)
{
    _complete = false;
    _type = type;

    switch (_type)
    {
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

    setAcceptDrops(true);

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
    layout->addSpacerItem(new QSpacerItem(10, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    setLayout(layout);

    registerField("file", fileEdit);
}

int FilePage::nextId() const
{
    switch (_type)
    {
    case PinListImporter::CSV:
        return PinListImporter::PagePDFResults;
    case PinListImporter::PDF:
        return PinListImporter::PagePDFResults;
    case PinListImporter::Table:
        return PinListImporter::PagePDFResults;
    default:
        return PinListImporter::PagePDFResults;
    }
}

void FilePage::initializePage()
{
    /*if (!field("file").toString().isEmpty())
        setFile(field("file").toString());*/
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

void FilePage::fileExplore()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString("Choose a %1 file").arg(_fileTitle), QString(),
                                                    QString("%1 (%2)").arg(_fileTitle).arg("*."+_suffixes.join(" *.")));
    if (!fileName.isEmpty())
    {
        setFile(fileName);
    }
}

void FilePage::checkEntry(const QString &text)
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

void FilePage::setFile(const QString &file)
{
    fileEdit->setText(file);
}

bool FilePage::isComplete() const
{
    return _complete;
}

QString FilePage::file() const
{
    return _file;
}
