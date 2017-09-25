#ifndef DATASHEETBOX_H
#define DATASHEETBOX_H

#include "pdf_extract_common.h"

#include <QRectF>
#include <QString>

class DATASHEET_EXTRACTOR_EXPORT DatasheetBox
{
public:
    DatasheetBox();

    qreal distanceToPoint(const QPointF &center) const;

    QString text;
    QRectF pos;
    bool associated;

    static bool isAlign(const DatasheetBox &label, const DatasheetBox &number);
};


#endif  // DATASHEETBOX_H
