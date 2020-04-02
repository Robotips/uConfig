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

#include "resultspage.h"

#include <QDebug>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

#include "pinlistimporter.h"

ResultsPage::ResultsPage()
    : QWizardPage(nullptr)
{
    QVBoxLayout *layout = new QVBoxLayout;
    _resultLabel = new QLabel();
    _resultLabel->setTextInteractionFlags(Qt::TextSelectableByKeyboard | Qt::TextSelectableByMouse);
    _resultLabel->setWordWrap(true);
    _resultLabel->setScaledContents(true);
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(_resultLabel);
    layout->addWidget(scrollArea);
    layout->addSpacerItem(new QSpacerItem(10, 30, QSizePolicy::Expanding, QSizePolicy::Expanding));
    setLayout(layout);
}

int ResultsPage::nextId() const
{
    return -1;
}

void ResultsPage::initializePage()
{
    QString filepdf = field("file").toString();
    QFileInfo info(filepdf);
    QString fileName = info.fileName();
    setTitle(QString("Extracted packages in %1").arg(fileName.right(30)));

    QString resText = QString("Packages found:");
    QList<Component *> &components = dynamic_cast<PinListImporter *>(wizard())->components();
    for (Component *component : components)
    {
        resText.append(QString("\n - %1 with %2 pins").arg(component->name()).arg(component->pins().count()));
    }
    _resultLabel->setText(resText);
}

bool ResultsPage::isComplete() const
{
    return true;
}
