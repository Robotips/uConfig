#ifndef DATASHEET_H
#define DATASHEET_H

#include "datasheet_extractor_common.h"

#include <QImage>

#include "../kicad/component.h"

namespace Poppler {
    class Document;
}

struct DATASHEET_EXTRACTOR_EXPORT DatasheetBox
{
    QString text;
    QRectF pos;
};

struct DATASHEET_EXTRACTOR_EXPORT DatasheetPin
{
    QString name;
    int pin;
    QRectF pos;
    QRectF numPos;
};

class DATASHEET_EXTRACTOR_EXPORT DatasheetPackage
{
public:
    DatasheetPackage();
    ~DatasheetPackage();

    Component toComponent() const;

public:
    QString name;
    QString packName;
    QStringList icname;
    QRectF rect;
    QRectF rectNum;
    QList<DatasheetPin> pins;
    QImage image;
};

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

    bool isAlign(DatasheetBox &label, DatasheetBox &number);
    QList<DatasheetPackage *> _packages;
};

#endif  // DATASHEET_H
