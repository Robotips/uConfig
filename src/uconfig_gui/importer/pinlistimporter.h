#ifndef PINLISTIMPORTER_H
#define PINLISTIMPORTER_H

#include <QWizard>

#include "datasheetresultspage.h"

class PinListImporter : public QWizard
{
    Q_OBJECT
public:

    enum ImportType
    {
        CSV,
        PDF,
        Table,
        Kicad
    };

    enum Page
    {
        PageStart,
        PageCSVFile,
        PagePDFFile, PagePDFResults,
        PageTableFile
    };

    PinListImporter(const QString &fileName, QWidget *parent = 0);

    Datasheet *datasheet();

private:
    DatasheetResultsPage *datasheetResult;
};

#endif // PINLISTIMPORTER_H
