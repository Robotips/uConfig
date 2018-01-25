#ifndef DATASHEET_H
#define DATASHEET_H

#include "pdf_extract_common.h"

#include <QObject>
#include <QImage>

#include "datasheetpackage.h"
#include "datasheetbox.h"
#include "../kicad/model/component.h"

namespace Poppler
{
class Document;
class Page;
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
    QList<Component *> components();

    QString name() const;

    bool debugEnabled() const;
    void setDebugEnabled(bool debug);

    bool forceEnabled() const;
    void setForceEnabled(bool force);

    int pageCount() const;
    QImage pageThumbnail(int numPage) const;

    void clean();

signals:
    void pageChanged(int page);
    void log(QString txt);

private:
    int pagePinDiagram(int pageStart, int pageEnd, bool *bgaStyle);

    void pinSearch(int numPage);

    Poppler::Document *_doc;
    QString _name;

    QList<DatasheetPackage *> _packages;
    bool _debug;
    bool _force;
};

#endif  // DATASHEET_H
