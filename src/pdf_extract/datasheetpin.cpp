#include "datasheetpin.h"

DatasheetPin::DatasheetPin()
{
}

DatasheetPin::~DatasheetPin()
{
    delete numberBox;
    delete nameBox;
}

qreal DatasheetPin::distanceToPoint(const QPointF &center) const
{
    qreal dist, newdist;

    dist = (center - pos.center()).manhattanLength();

    newdist = (center - pos.bottomLeft()).manhattanLength();
    if (newdist < dist)
        dist = newdist;

    newdist = (center - pos.topRight()).manhattanLength();
    if (newdist < dist)
        dist = newdist;

    newdist = (center - pos.bottomRight()).manhattanLength();
    if (newdist < dist)
        dist = newdist;

    newdist = (center - pos.topLeft()).manhattanLength();
    if (newdist < dist)
        dist = newdist;

    return dist;
}
