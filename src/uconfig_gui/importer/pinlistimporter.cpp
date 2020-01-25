#include "pinlistimporter.h"

#include "startwizardpage.h"
#include "filepage.h"
#include "pdffilepage.h"
#include "resultspage.h"
#include "datasheetprocesspage.h"
#include "componentspage.h"

#include <QAbstractButton>

PinListImporter::PinListImporter(const QString &fileName, QWidget *parent) :
    QWizard(parent)
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
    setOption(QWizard::DisabledBackButtonOnLastPage, true);
}

PinListImporter::~PinListImporter()
{
    try
    {
        _datasheetProcess->datasheetThread()->terminate();
        _datasheetProcess->datasheetThread()->wait();
    }
    catch (...) {
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
