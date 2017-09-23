#ifndef DATASHEETPACKAGE_H
#define DATASHEETPACKAGE_H

#include "datasheet_extractor_common.h"

#include "datasheetpin.h"
#include "../kicad/component.h"

#include <QString>
#include <QStringList>
#include <QRectF>
#include <QList>
#include <QImage>

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

#endif // DATASHEETPACKAGE_H
