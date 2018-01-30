/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef DRAWARC_H
#define DRAWARC_H

#include "../kicad_global.h"

#include "draw.h"

class KICAD_EXPORT DrawArc : public Draw
{
public:
    DrawArc();

    uint radius() const;
    void setRadius(const uint &radius);

    int startAngle() const;
    void setStartAngle(int startAngle);

    QPoint startPos() const;
    void setStartPos(const QPoint &startPos);

    int endAngle() const;
    void setEndAngle(int endAngle);

    QPoint endPos() const;
    void setEndPos(const QPoint &endPos);

protected:
    uint _radius;
    int _startAngle;
    QPoint _startPos;
    int _endAngle;
    QPoint _endPos;
};

#endif // DRAWARC_H
