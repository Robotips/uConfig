/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
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

#ifndef PINCLASS_H
#define PINCLASS_H

#include <QList>
#include <QRect>
#include <QString>
#include <QtCore/qglobal.h>

#include "classrule.h"
#include "pinclassitem.h"

#include "model/drawrect.h"
#include "model/drawtext.h"

class KICAD_EXPORT PinClass : public ClassRule
{
public:
    PinClass(QString className);

    QString className() const;

    void applyRule(ClassRule *rule);
    void applyRules(const QList<ClassRule *> &rules);

    void sortPins();

    void setPos(const QPoint &pos);
    QPoint pos() const;

    const QRect &boundingRect() const;

    void addPinItem(PinClassItem *pin);
    const QList<PinClassItem *> &pins() const;

    DrawText *getDrawText() const;
    DrawRect *getDrawRect() const;

protected:
    QString _className;
    QPoint _pos;

    void computeBoundingRect() const;
    mutable bool _brect;
    mutable QRect _boundingRect;

    QList<PinClassItem *> _pins;
};

#endif  // PINCLASS_H
