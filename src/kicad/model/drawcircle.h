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

#ifndef DRAWCIRCLE_H
#define DRAWCIRCLE_H

#include "../kicad_global.h"

#include "draw.h"

class KICAD_EXPORT DrawCircle : public Draw
{
public:
    DrawCircle();

    TypeDraw type() const;

    uint radius() const;
    void setRadius(const uint &radius);

protected:
    uint _radius;
};

#endif // DRAWCIRCLE_H
