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

#include "rule.h"

#include <QDebug>

Rule::Rule(const QString &selector)
    : _selector(selector, QRegularExpression::CaseInsensitiveOption)
{
    _enabled = true;
}

Rule::~Rule()
{
}

QRegularExpression Rule::selector() const
{
    return _selector;
}

void Rule::setSelector(const QRegularExpression &selector)
{
    _selector = selector;
}

void Rule::setSelector(const QString &selector)
{
    _selector.setPattern(selector);
    _selector.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
}

bool Rule::matchWithName(const QString &name) const
{
    if (!_enabled)
    {
        return false;
    }
    if (!_selector.isValid())
    {
        return false;
    }

    QRegularExpressionMatch match = _selector.match(name, 0, QRegularExpression::NormalMatch);
    return (match.hasMatch() /*&& match.capturedEnd() == name.size()*/);
}

bool Rule::isEnabled() const
{
    return _enabled;
}

void Rule::setEnabled(bool enabled)
{
    _enabled = enabled;
}

QString Rule::file() const
{
    return _file;
}

void Rule::setFile(const QString &file)
{
    _file = file;
}

int Rule::line() const
{
    return _line;
}

void Rule::setLine(int line)
{
    _line = line;
}
