#ifndef PINLISTIMPORTER_H
#define PINLISTIMPORTER_H

#include <QWizard>

#include "datasheetprocesspage.h"

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
        PagePDFFile, PagePDFProcess, PagePDFResults,
        PageTableFile
    };

    PinListImporter(const QString &fileName, QWidget *parent = 0);

    Datasheet *datasheet();

private:
    DatasheetProcessPage *datasheetProcess;
};

#endif // PINLISTIMPORTER_H
