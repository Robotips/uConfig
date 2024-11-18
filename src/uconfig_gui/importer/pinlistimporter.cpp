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

#include "pinlistimporter.h"

#include "componentspage.h"
#include "datasheetprocesspage.h"
#include "filepage.h"
#include "pdffilepage.h"
#include "resultspage.h"
#include "startwizardpage.h"

#include <QAbstractButton>

PinListImporter::PinListImporter(const QString &fileName, QWidget *parent)
    : QWizard(parent),
      _type(Undefined)
{
    setPage(PageStart, new StartWizardPage());
    setPage(PageFile, new FilePage());

    // PDF
    _datasheetProcess = new DatasheetProcessPage();
    setPage(PagePDFFile, new PDFFilePage(_datasheetProcess->datasheetThread()));
    setPage(PagePDFProcess, _datasheetProcess);

    // Components
    setPage(PageComponents, new ComponentsPage());

    // Results
    setPage(PageResults, new ResultsPage());

    if (fileName.isEmpty())
    {
        setStartId(PageStart);
    }
    else if (fileName.endsWith(".pdf", Qt::CaseInsensitive))
    {
        setField("file", fileName);
        setStartId(PagePDFFile);
        _type = PDF;
    }
    else if (fileName.endsWith(".lib", Qt::CaseInsensitive))
    {
        setField("file", fileName);
        setStartId(PageComponents);
        _type = Kicad;
    }
    else if (fileName.endsWith(".csv", Qt::CaseInsensitive))
    {
        setField("file", fileName);
        setStartId(PageComponents);
        _type = CSV;
    }
    setOption(QWizard::DisabledBackButtonOnLastPage, true);
}

PinListImporter::~PinListImporter()
{
    try
    {
        _datasheetProcess->datasheetThread()->terminate();
        _datasheetProcess->datasheetThread()->wait();
    }
    catch (...)
    {
    }
    _datasheetProcess->datasheetThread()->deleteLater();
}

QList<Component *> &PinListImporter::components()
{
    return _components;
}

QString PinListImporter::filePath() const
{
    return _filePath;
}

void PinListImporter::setFilePath(const QString &filePath)
{
    _filePath = filePath;
}

PinListImporter::ImportType PinListImporter::type()
{
    return _type;
}

void PinListImporter::setType(PinListImporter::ImportType type)
{
    _type = type;
}
