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

#ifndef PINRULE_H
#define PINRULE_H

#include <QString>
#include <QStringList>
#include <QtCore/qglobal.h>

#include "model/pin.h"
#include "rule.h"

class KICAD_EXPORT PinRule : public Rule
{
public:
    PinRule(const QString &selector = QString(".*"));

    Type type() const override;

    bool setProperty(const QString &name, const QString &value) override;
    bool hasPropertySet(const QString &name) const override;

    // class property
    QString className(const QString &pinName = QString()) const;
    void setClassName(const QString &className);
    bool hasClassSet() const;

    // elecType property
    Pin::ElectricalType elecType() const;
    void setElecType(Pin::ElectricalType elecType);
    void setElecType(const QString &className);
    bool hasElecTypeSet() const;

    // pinType property
    Pin::PinType pinType() const;
    void setPinType(const Pin::PinType &pinType);
    void setPinType(const QString &className);
    bool hasPinTypeSet() const;

    // priority property
    int priority() const;
    void setPriority(int priority);
    bool hasPrioritySet() const;

    static QStringList propsName;
    static QStringList elecTypeEnumStr;
    static QStringList pinTypeEnumStr;

protected:
    QString _className;
    bool _classSet;

    Pin::ElectricalType _elecType;
    bool _elecTypeSet;

    Pin::PinType _pinType;
    bool _pinTypeSet;

    int _priority;
    bool _prioritySet;
};

#endif  // PINRULE_H
