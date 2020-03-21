#ifndef PINLISTIMPORTER_H
#define PINLISTIMPORTER_H

#include <QWizard>

#include "../kicad/model/component.h"

class DatasheetProcessPage;

class PinListImporter : public QWizard
{
    Q_OBJECT
public:
    PinListImporter(const QString &fileName, QWidget *parent = nullptr);
    ~PinListImporter() override;

    /**
     * When extending this enum make sure to extend
     * FilePage::_importTypeSettingsKeymap in the filepage.cpp as well!
     */
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
        PagePDFFile,
        PagePDFProcess,
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
    DatasheetProcessPage *_datasheetProcess;
};

#endif // PINLISTIMPORTER_H
