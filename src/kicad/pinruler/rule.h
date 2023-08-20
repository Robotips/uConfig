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

#ifndef RULE_H
#define RULE_H

#include <QRegularExpression>
#include <QString>
#include <QtCore/qglobal.h>

class KICAD_EXPORT Rule
{
public:
    Rule(const QString &selector = QString(".*"));
    virtual ~Rule();

    enum Type
    {
        PinRuleType,
        ClassRuleType
    };
    virtual Type type() const = 0;

    QRegularExpression selector() const;
    void setSelector(const QRegularExpression &selector);
    void setSelector(const QString &selector);
    bool matchWithName(const QString &name) const;

    bool isEnabled() const;
    void setEnabled(bool enabled);

    virtual bool setProperty(const QString &name, const QString &value) = 0;
    virtual bool hasPropertySet(const QString &name) const = 0;

    // debug
    QString file() const;
    void setFile(const QString &file);

    int line() const;
    void setLine(int line);

protected:
    QRegularExpression _selector;
    bool _enabled;

    // debug purpose
    QString _file;
    int _line;
};

#endif  // RULE_H
