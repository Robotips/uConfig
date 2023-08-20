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

#include "startwizardpage.h"

#include <QButtonGroup>
#include <QFormLayout>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QVariant>

#include "pinlistimporter.h"

StartWizardPage::StartWizardPage(QWidget *parent)
    : QWizardPage(parent)
{
    _complete = false;
    setAcceptDrops(true);

    setTitle("Choose import format");
    setSubTitle("This wizard will help you import a pin list from various file source");

    QLabel *label = new QLabel("Format:");
    label->setWordWrap(true);

    QButtonGroup *group = new QButtonGroup;
    group->setExclusive(true);
    connect(group, &QButtonGroup::idClicked, this, &StartWizardPage::buttonClick);

    QToolButton *buttonKicad = new QToolButton;
    buttonKicad->setIcon(QIcon(":/icons/img/kicad-lib"));
    buttonKicad->setToolTip("Kicad library");
    buttonKicad->setIconSize(QSize(64, 64));
    buttonKicad->setCheckable(true);
    group->addButton(buttonKicad, PinListImporter::Kicad);

    QToolButton *buttonPdf = new QToolButton;
    buttonPdf->setIcon(QIcon(":/icons/img/application-pdf"));
    buttonPdf->setToolTip("PDF datasheet");
    buttonPdf->setIconSize(QSize(64, 64));
    buttonPdf->setCheckable(true);
    group->addButton(buttonPdf, PinListImporter::PDF);

    /*QToolButton *buttonTable = new QToolButton;
    buttonTable->setIcon(QIcon(":/icons/img/application-x-applix-spreadsheet"));
    buttonTable->setToolTip("XLS excel file");
    buttonTable->setIconSize(QSize(64, 64));
    buttonTable->setCheckable(true);
    //buttonTable->setEnabled(false);
    group->addButton(buttonTable, PinListImporter::Table);*/

    QToolButton *buttonCsv = new QToolButton;
    buttonCsv->setIcon(QIcon(":/icons/img/text-csv"));
    buttonCsv->setToolTip("CSV file");
    buttonCsv->setIconSize(QSize(64, 64));
    buttonCsv->setCheckable(true);
    group->addButton(buttonCsv, PinListImporter::CSV);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(buttonKicad, new QLabel("Kicad library\nImports components from existing kicad library"));
    formLayout->addRow(buttonPdf, new QLabel("PDF datasheet\nExtracts components from PDF datasheet"));
    // formLayout->addRow(buttonTable, new QLabel("Table excel"));
    formLayout->addRow(buttonCsv, new QLabel("CSV coma separator"));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addLayout(formLayout);
    setLayout(layout);
}

int StartWizardPage::nextId() const
{
    return PinListImporter::PageFile;
}

bool StartWizardPage::isComplete() const
{
    return _complete;
}

void StartWizardPage::buttonClick(int type)
{
    dynamic_cast<PinListImporter *>(wizard())->setType((PinListImporter::ImportType)type);
    _complete = true;
    emit completeChanged();
    wizard()->next();
}
