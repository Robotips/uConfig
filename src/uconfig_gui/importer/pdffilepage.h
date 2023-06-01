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

#ifndef PDFFILEPAGE_H
#define PDFFILEPAGE_H

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QWizardPage>
#include <QSpinBox>

#include "datasheetthread.h"

#include "pinlistimporter.h"

class PDFFilePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit PDFFilePage(DataSheetThread *datasheetThread);

    bool isComplete() const override;
    int nextId() const override;
    void initializePage() override;

private slots:
    void check();

private:
    DataSheetThread *_datasheetThread;
    bool _complete;
    QLabel *_pagePreviewLabel;
    QRadioButton *_allRadio;
    QRadioButton *_partialRadio;
    QLineEdit *_rangeEdit, *_deleteString;
    QSpinBox *_maxPinNameLength;
    QCheckBox *_forceCheckBox;
    QLabel *_pageCountLabel;

    QStringList _suffixes;
    int _s=0;
};

#endif  // PDFFILEPAGE_H
