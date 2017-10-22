#ifndef DATASHEET_H
#define DATASHEET_H

#include "pdf_extract_common.h"

#include <QObject>
#include <QImage>

#include "datasheetpackage.h"
#include "datasheetbox.h"
#include "../kicad/component.h"

namespace Poppler
{
class Document;
}

class DATASHEET_EXTRACTOR_EXPORT Datasheet : public QObject
{
    Q_OBJECT
public:
    Datasheet();
    ~Datasheet();

    bool open(QString fileName);
    void close();

    void analyse(int pageBegin = -1, int pageEnd = -1);

    const QList<DatasheetPackage *> &packages() const;

    QString name() const;

    bool debugEnabled() const;
    void setDebugEnabled(bool debug);

    int pageCount() const;
    QImage pageThumbnail(int numPage) const;

signals:
    void pageChanged(int page);
    void log(QString txt);

private:
    int pagePinDiagram(int pageStart, bool *bgaStyle);

    void pinSearchBGA(int numPage);
    void pinSearch(int numPage);

    Poppler::Document *_doc;
    QString _name;

    QList<DatasheetPackage *> _packages;
    bool _debug;
};

#endif  // DATASHEET_H
