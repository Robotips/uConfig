#ifndef PINLISTIMPORTER_H
#define PINLISTIMPORTER_H

#include <QWizard>

#include "../kicad/component.h"

class PinListImporter : public QWizard
{
    Q_OBJECT
public:

    PinListImporter(const QString &fileName, QWidget *parent = 0);
    ~PinListImporter();

    enum ImportType
    {
        CSV,
        PDF,
        Table,
        Kicad
    };
    ImportType type();
    void setType(ImportType type);

    QString filePath() const;
    void setFilePath(const QString &filePath);

    enum Page
    {
        PageStart,
        PageFile,
        PagePDFFile, PagePDFProcess,
        PageCSVFile,
        PageKicadFile,
        PageComponents,
        PageResults
    };

    QList<Component *> &components();

private:
    ImportType _type;
    QString _filePath;
    QList<Component *> _components;
};

#endif // PINLISTIMPORTER_H
