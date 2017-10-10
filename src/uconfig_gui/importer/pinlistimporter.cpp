#include "pinlistimporter.h"

#include "startwizardpage.h"
#include "filepage.h"
#include "datasheetresultspage.h"

PinListImporter::PinListImporter(const QString &fileName, QWidget *parent) :
    QWizard(parent)
{
    setPage(PageStart, new StartWizardPage());
    //setPage(PageCSVFile, new FilePage(CSV));

    setPage(PagePDFFile, new FilePage(PDF));
    datasheetProcess = new DatasheetProcessPage();
    setPage(PagePDFProcess, datasheetProcess);
    setPage(PagePDFResults, new DatasheetResultsPage(datasheetProcess->datasheet()));

    //setPage(PageTableFile, new FilePage(Table));

    if (fileName.isEmpty())
    {
        setStartId(PageStart);
    }
    else if(fileName.endsWith(".pdf"))
    {
        setField("file", fileName);
        setStartId(PagePDFFile);
    }
}

Datasheet *PinListImporter::datasheet()
{
    return datasheetProcess->datasheet();
}
