#ifndef DATASHEET_H
#define DATASHEET_H

#include <poppler/qt5/poppler-qt5.h>
#include <poppler/qt5/poppler-form.h>
#include <QImage>

#include "datasheet_extractor_common.h"

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

public:
    QString name;
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
