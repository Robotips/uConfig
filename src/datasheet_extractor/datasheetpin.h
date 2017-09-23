#ifndef DATASHEETPIN_H
#define DATASHEETPIN_H

#include "datasheet_extractor_common.h"

#include <QString>
#include <QRectF>

class DATASHEET_EXTRACTOR_EXPORT DatasheetPin
{
public:
    DatasheetPin();

    QString name;
    int pin;
    QRectF pos;
    QRectF numPos;
};

#endif // DATASHEETPIN_H
