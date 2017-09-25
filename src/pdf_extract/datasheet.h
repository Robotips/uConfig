#ifndef DATASHEET_H
#define DATASHEET_H

#include "pdf_extract_common.h"

#include "datasheetpackage.h"
#include "datasheetbox.h"
#include "../kicad/component.h"

namespace Poppler {
    class Document;
}

class DATASHEET_EXTRACTOR_EXPORT Datasheet
{
public:
    Datasheet();
    ~Datasheet();

    bool open(int dsid);
    bool open(QString fileName);
    void close();

    void analyse();

    const QList<DatasheetPackage *> &packages() const;

private:
    int pagePinDiagram(int pageStart, bool *bgaStyle);

    void pinSearchBGA(int numPage);
    void pinSearch(int numPage);

    Poppler::Document *_doc;

    QList<DatasheetPackage *> _packages;
};

#endif  // DATASHEET_H
