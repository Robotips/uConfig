#ifndef PINLISTIMPORTER_H
#define PINLISTIMPORTER_H

#include <QWizard>

#include "datasheetprocesspage.h"

class PinListImporter : public QWizard
{
    Q_OBJECT
public:

    PinListImporter(const QString &fileName, QWidget *parent = 0);

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
        PagePDFFile, PagePDFProcess, PagePDFResults,
        PageCSVFile, PageCSVResults,
        PageKicadFile, PageKicadResults
    };

    QList<Component *> components();

private:
    ImportType _type;
    QList<Component *> _components;
};

#endif // PINLISTIMPORTER_H
