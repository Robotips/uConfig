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
    ImportType type();
    void setType(ImportType type);

    enum Page
    {
        PageStart,
        PageFile,
        PagePDFFile, PagePDFProcess, PagePDFResults
    };

    PinListImporter(const QString &fileName, QWidget *parent = 0);

    Datasheet *datasheet();

private:
    DatasheetProcessPage *datasheetProcess;
    ImportType _type;
};

#endif // PINLISTIMPORTER_H
