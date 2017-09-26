#ifndef DATASHEETPIN_H
#define DATASHEETPIN_H

#include "pdf_extract_common.h"

#include "datasheetbox.h"

#include <QString>
#include <QRectF>

class DATASHEET_EXTRACTOR_EXPORT DatasheetPin
{
public:
    DatasheetPin();

    qreal distanceToPoint(const QPointF &center) const;

    QString name;
    int pin;
    QRectF pos;
    QRectF numPos;

    DatasheetBox *numberBox;
    DatasheetBox *nameBox;
};

#endif // DATASHEETPIN_H
