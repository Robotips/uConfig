#include "pinlistimporter.h"

#include "startwizardpage.h"
#include "filepage.h"

PinListImporter::PinListImporter(const QString &fileName, QWidget *parent) :
    QWizard(parent)
{
    setPage(PageStart, new StartWizardPage());
    //setPage(PageCSVFile, new FilePage(CSV));

    setPage(PagePDFFile, new FilePage(PDF));
    datasheetResult = new DatasheetResultsPage();
    setPage(PagePDFResults, datasheetResult);

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
    return datasheetResult->datasheet();
}
