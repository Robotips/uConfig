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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QImage>
#include <QList>
#include <QRect>
#include <QString>
#include <QStringList>

#include "draw.h"
#include "drawtext.h"
#include "pin.h"

/**
 * @brief The Component class contains all information about one schematic component.
 * It could also have alias name, stored in the same component.
 */
class KICAD_EXPORT Component
{
public:
    Component(const QString &name = QString());
    Component(const Component &other);
    ~Component();

    const QString &name() const;
    void setName(const QString &name);

    QList<Pin *> &pins();
    const QList<Pin *> &pins() const;
    void addPin(Pin *pin);
    void removePin(Pin *pin);
    void clearPins();

    QList<Draw *> &draws();
    const QList<Draw *> &draws() const;
    void prependDraw(Draw *draw);
    void addDraw(Draw *draw);
    void removeDraw(Draw *draw);
    void clearDraws();

    const QString &prefix() const;
    void setPrefix(const QString &prefix);

    QStringList &aliases();
    const QStringList &aliases() const;
    void addAlias(const QString &alias);
    void addAlias(const QStringList &aliases);

    QStringList &footPrints();
    const QStringList &footPrints() const;
    void addFootPrint(const QString &footprint);
    void addFootPrint(const QStringList &footprints);

    QRect boundingRect() const;
    QRect rect() const;
    void setRect(const QRect &rect);

    bool showPinName() const;
    void setShowPinName(bool showPinName);

    bool showPadName() const;
    void setShowPadName(bool showPadName);

    int unitCount() const;
    void setUnitCount(int unitCount);

    const QImage &debugInfo() const;
    void setDebugInfo(const QImage &debugInfo);

    DrawText *refText() const;
    void setRefText(DrawText *refText);

    DrawText *nameText() const;
    void setNameText(DrawText *nameText);

    DrawText *packageText() const;
    void setPackageText(DrawText *packageText);

    DrawText *docText() const;
    void setDocText(DrawText *docText);

    void sort();
    void reorganizeToPackageStyle();

private:
    QList<Pin *> _pins;
    QList<Draw *> _draws;

    QString _name;
    QString _prefix;
    QStringList _alias;
    QStringList _footPrints;
    bool _showPinName;
    bool _showPadName;
    int _unitCount;
    QImage _debugInfo;
    QRect _rect;
    DrawText *_refText;
    DrawText *_nameText;
    DrawText *_packageText;
    DrawText *_docText;
};

#endif  // COMPONENT_H
