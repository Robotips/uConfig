#ifndef DATASHEETBOX_H
#define DATASHEETBOX_H

#include "datasheet_extractor_common.h"

#include <QRectF>
#include <QString>

class DATASHEET_EXTRACTOR_EXPORT DatasheetBox
{
public:
    DatasheetBox();

    QString text;
    QRectF pos;

    qreal distanceToPoint(const QPointF &center) const;

    static bool isAlign(const DatasheetBox &label, const DatasheetBox &number);
};


#endif  // DATASHEETBOX_H
