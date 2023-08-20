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

#include "pdffilepage.h"

#include <QDebug>
#include <QFileInfo>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QRegularExpressionValidator>

#include "pinlistimporter.h"

PDFFilePage::PDFFilePage(DataSheetThread *datasheetThread)
    : QWizardPage(nullptr),
      _datasheetThread(datasheetThread)
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
    connect(_allRadio, &QRadioButton::toggled, this, &PDFFilePage::check);
    rangeGroupLayout->addWidget(_allRadio);

    _partialRadio = new QRadioButton(tr("Page range"));
    rangeGroupLayout->addWidget(_partialRadio);

    _rangeEdit = new QLineEdit();
    _rangeEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]+(\\-[0-9]+)?$")));
    connect(_rangeEdit, &QLineEdit::textChanged, this, &PDFFilePage::check);
    registerField("pdfpagerange", _rangeEdit);
    rangeGroupLayout->addWidget(_rangeEdit);

    _forceCheckBox = new QCheckBox(tr("Force détection\n(try to build component in each page,\nvery expensive)"));
    _allRadio->setChecked(true);
    connect(_forceCheckBox, &QCheckBox::toggled, this, &PDFFilePage::check);
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

    if (_complete)
    {
        _datasheetThread->setForceEnabled(_forceCheckBox->isChecked() && !_allRadio->isChecked());
        if (_allRadio->isChecked())
        {
            _datasheetThread->setRange();
        }
        else
        {
            int start = -1;
            int stop = -1;
            QRegularExpression reg("^([0-9]+)(\\-[0-9]+)?$");
            QRegularExpressionMatch match = reg.match(_rangeEdit->text());
            start = match.captured(1).toInt() - 1;
            if (start >= _datasheetThread->datasheet()->pageCount() || start < 0)
            {
                _complete = false;
            }
            if (match.capturedTexts().count() > 2)
            {
                stop = -match.captured(2).toInt() - 1;
                if (stop >= _datasheetThread->datasheet()->pageCount() || stop < 0)
                {
                    _complete = false;
                }
                if (start > stop)
                {
                    _complete = false;
                }
            }
            _datasheetThread->setRange(start, stop);
        }
    }
    emit completeChanged();
}

bool PDFFilePage::isComplete() const
{
    return _complete;
}
