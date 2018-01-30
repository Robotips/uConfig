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

#ifndef COMPONENT_H
#define COMPONENT_H

#include "../kicad_global.h"

#include <QList>
#include <QString>
#include <QRect>
#include <QStringList>
#include <QImage>

#include "pin.h"
#include "draw.h"

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

    QList<Draw *> &draws();
    const QList<Draw *> &draws() const;
    void addDraw(Draw *draw);
    void removeDraw(Draw *draw);

    const QString &prefix() const;
    void setPrefix(const QString &prefix);

    const QStringList &aliases() const;
    void addAlias(const QString &alias);

    QStringList &footPrints();
    const QStringList &footPrints() const;
    void addFootPrint(const QString &footprint);

    const QRect &rect() const;
    void setRect(const QRect &rect);

    bool showPinName() const;
    void setShowPinName(bool showPinName);

    bool showPadName() const;
    void setShowPadName(bool showPadName);

    void sort();
    void reorganizeToPackageStyle();

    const QImage &debugInfo() const;
    void setDebugInfo(const QImage &debugInfo);

    bool isValid() const;

    friend QTextStream &operator<<(QTextStream &stream, const Component &component);
    friend QTextStream &operator>>(QTextStream &stream, Component &component);

private:
    QList<Pin *> _pins;
    QList<Draw *> _draws;

    QString _name;
    QString _prefix;
    QStringList _alias;
    QStringList _footPrints;
    QRect _rect;
    bool _valid;
    bool _showPinName;
    bool _showPadName;
    QImage _debugInfo;
};

#endif  // COMPONENT_H
