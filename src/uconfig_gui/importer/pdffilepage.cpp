#include "pdffilepage.h"

#include <QLabel>
#include <QGroupBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QRegularExpressionValidator>

#include "pinlistimporter.h"

PDFFilePage::PDFFilePage(DataSheetThread *datasheetThread) :
    QWizardPage(0), _datasheetThread(datasheetThread)
{
    _complete = false;

    // previewGroup
    QGroupBox *previewGroup = new QGroupBox(tr("preview"));
    QVBoxLayout *previewGroupLayout = new QVBoxLayout;

    _pagePreviewLabel = new QLabel();
    _pagePreviewLabel->setAlignment(Qt::AlignHCenter);
    previewGroupLayout->addWidget(_pagePreviewLabel);

    previewGroup->setLayout(previewGroupLayout);

    // rangeGroup
    QGroupBox *rangeGroup = new QGroupBox(tr("page selection"));
    QVBoxLayout *rangeGroupLayout = new QVBoxLayout;

    _allRadio = new QRadioButton(tr("Whole document (could be long)"));
    connect(_allRadio, SIGNAL(toggled(bool)), this, SLOT(check()));
    rangeGroupLayout->addWidget(_allRadio);

    _partialRadio = new QRadioButton(tr("Page range"));
    rangeGroupLayout->addWidget(_partialRadio);

    _rangeEdit = new QLineEdit();
    _rangeEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]+(\\-[0-9]+)?$")));
    connect(_rangeEdit, SIGNAL(textChanged(QString)), this, SLOT(check()));
    registerField("pdfpagerange", _rangeEdit);
    rangeGroupLayout->addWidget(_rangeEdit);

    _forceCheckBox = new QCheckBox(tr("Force détection\n(try to build component in each page,\nvery expensive)"));
    _allRadio->setChecked(true);
    rangeGroupLayout->addWidget(_forceCheckBox);
    rangeGroupLayout->addSpacerItem(new QSpacerItem(10, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));

    _pageCountLabel = new QLabel();
    rangeGroupLayout->addWidget(_pageCountLabel);

    rangeGroup->setLayout(rangeGroupLayout);

    // global layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(rangeGroup);
    layout->addWidget(previewGroup);
    setLayout(layout);
}

int PDFFilePage::nextId() const
{
    return PinListImporter::PagePDFProcess;
}

void PDFFilePage::initializePage()
{
    QString filepdf = field("file").toString();
    QFileInfo info(filepdf);
    QString fileName = info.fileName();
    setTitle(QString("%1 selection pages").arg(fileName.right(30)));
    _datasheetThread->open(filepdf);
    _pagePreviewLabel->setPixmap(QPixmap::fromImage(_datasheetThread->datasheet()->pageThumbnail(0)));
    _rangeEdit->setText(QString("1-%1").arg(_datasheetThread->datasheet()->pageCount()));
    _pageCountLabel->setText(QString("%1 pages").arg(_datasheetThread->datasheet()->pageCount()));
}

void PDFFilePage::check()
{
    _forceCheckBox->setEnabled(!_allRadio->isChecked());
    _rangeEdit->setEnabled(!_allRadio->isChecked());
    _complete = _rangeEdit->hasAcceptableInput() || _allRadio->isChecked();
    emit completeChanged();

    if (_complete)
    {
        _datasheetThread->setForceEnabled(_forceCheckBox->isChecked() && !_allRadio->isChecked());
        if (_allRadio->isChecked())
            _datasheetThread->setRange();
        else
        {
            int start=-1, stop=-1;
            QRegularExpression reg("^([0-9]+)(\\-[0-9]+)?$");
            QRegularExpressionMatch match = reg.match(_rangeEdit->text());
            start = match.captured(1).toInt()-1;
            if (match.capturedTexts().count()>2)
                stop = -match.captured(2).toInt()-1;
            _datasheetThread->setRange(start, stop);
        }
    }
}

bool PDFFilePage::isComplete() const
{
    return _complete;
}
