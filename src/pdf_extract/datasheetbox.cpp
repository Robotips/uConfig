#include "datasheetbox.h"

DatasheetBox::DatasheetBox()
    : associated(false)
{
}

qreal DatasheetBox::distanceToPoint(const QPointF &center) const
{
    qreal dist, newdist;

    dist = (center - pos.center()).manhattanLength();

    newdist = (center - pos.bottomLeft()).manhattanLength();
    if (newdist < dist)
        dist = newdist;

    newdist = (center - pos.topRight()).manhattanLength();
    if (newdist < dist)
        dist = newdist;
    newdist = (center - QPointF(pos.left(), pos.center().y())).manhattanLength();
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

bool DatasheetBox::isAlign(const DatasheetBox &label, const DatasheetBox &number)
{
    if (label.pos.width() > label.pos.height() || label.text.count()<3)  // Horizontal
    {
        qreal marge = label.pos.height();
        if (label.pos.height() > number.pos.height()*2
         || number.pos.height() > label.pos.height()*2)
            return false;
        if (label.pos.top() - marge < number.pos.top() &&
            label.pos.bottom() + marge > number.pos.bottom())
            return true;
        else
            return false;
    }
    else  // Vertical
    {
        qreal marge = label.pos.width();
        if (label.pos.left() - marge < number.pos.left() &&
            label.pos.right() + marge > number.pos.right())
            return true;
        else
            return false;
    }
}
