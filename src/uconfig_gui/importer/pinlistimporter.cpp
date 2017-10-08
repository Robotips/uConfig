#include "pinlistimporter.h"

#include "startwizardpage.h"
#include "filepage.h"
//#include "datasheetresultspage.h"

PinListImporter::PinListImporter(QWidget *parent) :
    QWizard(parent)
{
    setPage(PageStart, new StartWizardPage());

    setPage(PageCSVFile, new FilePage(CSV));

    setPage(PagePDFFile, new FilePage(PDF));
    datasheetResult = new DatasheetResultsPage();
    setPage(PagePDFResults, datasheetResult);

    setPage(PageTableFile, new FilePage(Table));
}

Datasheet *PinListImporter::datasheet()
{
    return datasheetResult->datasheet();
}
