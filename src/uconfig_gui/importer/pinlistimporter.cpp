#include "pinlistimporter.h"

#include "startwizardpage.h"
#include "filepage.h"
#include "datasheetresultspage.h"

PinListImporter::PinListImporter(const QString &fileName, QWidget *parent) :
    QWizard(parent)
{
    setPage(PageStart, new StartWizardPage());
    setPage(PageFile, new FilePage());

    datasheetProcess = new DatasheetProcessPage();
    setPage(PagePDFProcess, datasheetProcess);
    setPage(PagePDFResults, new DatasheetResultsPage(datasheetProcess->datasheet()));

    if (fileName.isEmpty())
    {
        setStartId(PageStart);
    }
    else if(fileName.endsWith(".pdf"))
    {
        setField("file", fileName);
        setStartId(PageFile);
        _type = PDF;
    }
}

PinListImporter::ImportType PinListImporter::type()
{
    return _type;
}

void PinListImporter::setType(PinListImporter::ImportType type)
{
    _type = type;
}

Datasheet *PinListImporter::datasheet()
{
    return datasheetProcess->datasheet();
}
