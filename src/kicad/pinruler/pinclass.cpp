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

#include "pinclass.h"

#include <QDebug>
#include <qmath.h>
#include <utility>

#include "viewer/kicadfont.h"

PinClass::PinClass(QString className)
    : _className(std::move(className))
{
    _brect = false;
}

QString PinClass::className() const
{
    return _className;
}

void PinClass::applyRule(ClassRule *rule)
{
    if (!_positionSet && rule->hasPositionSet())
    {
        setPosition(rule->positionValue());
    }
    if (!_sortSet && rule->hasSortSet())
    {
        setSort(rule->sortValue());
    }
    if (!_sortPatternSet && rule->hasSortPatternSet())
    {
        QString newSortPatern = rule->sortPattern();
        QRegularExpressionMatch match = rule->selector().match(_className, 0, QRegularExpression::NormalMatch);
        if (match.hasMatch())
        {
            const QStringList &captures = (match.capturedTexts());
            if (captures.count() > 1)
            {
                newSortPatern.replace("\\1", captures[1]);
            }
            if (captures.count() > 2)
            {
                newSortPatern.replace("\\2", captures[2]);
            }
            if (captures.count() > 3)
            {
                newSortPatern.replace("\\3", captures[3]);
            }
        }
        setSortPattern(newSortPatern);
    }
    if (!_labelSet && rule->hasLabelSet())
    {
        QString newLabel = rule->label();
        QRegularExpressionMatch match = rule->selector().match(_className, 0, QRegularExpression::NormalMatch);
        if (match.hasMatch())
        {
            const QStringList &captures = (match.capturedTexts());
            if (captures.count() > 1)
            {
                newLabel.replace("\\1", captures[1]);
            }
            if (captures.count() > 2)
            {
                newLabel.replace("\\2", captures[2]);
            }
            if (captures.count() > 3)
            {
                newLabel.replace("\\3", captures[3]);
            }
        }
        setLabel(newLabel);
    }
    if (!_lengthSet && rule->hasLengthSet())
    {
        setLength(rule->length());
    }
    if (!_prioritySet && rule->hasPrioritySet())
    {
        setPriority(rule->priority());
    }
    if (!_visibilitySet && rule->hasVisibilitySet())
    {
        setVisibility(rule->visibilityValue());
    }
    if (!_rectSet && rule->hasRectSet())
    {
        setRect(static_cast<int>(rule->rect()));
    }
}

void PinClass::applyRules(const QList<ClassRule *> &rules)
{
    for (ClassRule *rule : rules)
    {
        applyRule(rule);
    }
}

void PinClass::sortPins()
{
    if (_sort == ClassRule::SortNone)
    {
        return;
    }

    QRegularExpression pattern(_sortPattern, QRegularExpression::CaseInsensitiveOption);
    QRegularExpression numPattern("([^0-9]*)([0-9]+)([^0-9]*)", QRegularExpression::CaseInsensitiveOption);

    for (PinClassItem *pinItem : _pins)
    {
        QString sortPatern;
        QString pinName = pinItem->pin()->name();
        pinName.replace("~", "");

        if (pattern.isValid())
        {
            QRegularExpressionMatch match = pattern.match(pinName);
            if (match.hasMatch())
            {
                if (match.lastCapturedIndex() > 0)
                {
                    sortPatern = match.captured(1);
                }
                else
                {
                    sortPatern = match.captured(0);
                }
            }
            else
            {
                sortPatern = pinName;
            }
        }
        else
        {
            sortPatern = pinName;
        }
        // sortPatern = sortPatern.leftJustified(n, ' ');

        QRegularExpressionMatchIterator numMatchIt = numPattern.globalMatch(sortPatern);
        if (numMatchIt.hasNext())
        {
            sortPatern.clear();
        }
        while (numMatchIt.hasNext())
        {
            QRegularExpressionMatch numMatch = numMatchIt.next();
            sortPatern.append(numMatch.captured(1) + QString('0').repeated(5 - numMatch.captured(2).size()) + numMatch.captured(2) + numMatch.captured(3));
        }
        QRegularExpressionMatchIterator padMatchIt = numPattern.globalMatch(pinItem->pin()->padName());
        while (padMatchIt.hasNext())
        {
            QRegularExpressionMatch padMatch = padMatchIt.next();
            sortPatern.append(padMatch.captured(1) + QString('0').repeated(5 - padMatch.captured(2).size()) + padMatch.captured(2) + padMatch.captured(3));
        }

        pinItem->setSortLabel(sortPatern);
    }

    if (_sort == ClassRule::SortAsc)
    {
        std::sort(_pins.begin(), _pins.end(), PinClassItem::pinLessThan);
    }
    if (_sort == ClassRule::SortDesc)
    {
        std::sort(_pins.begin(), _pins.end(), PinClassItem::pinGreaterThan);
    }
}

void PinClass::setPos(const QPoint &basePos)
{
    _pos = basePos;

    QPoint pinPos = basePos;
    QPoint offset;
    QPoint translate;
    int angle;

    switch (_position)
    {
        case ClassRule::PositionTop:
            angle = 270;
            offset = QPoint(100, 0);
            translate = QPoint(0, -_length);
            break;
        case ClassRule::PositionBottom:
            angle = 90;
            offset = QPoint(100, 0);
            translate = QPoint(0, _length);
            break;
        case ClassRule::PositionLeft:
            angle = 0;
            offset = QPoint(0, 100);
            translate = QPoint(-_length, 0);
            break;
        case ClassRule::PositionRight:
        case ClassRule::PositionASide:
            angle = 180;
            offset = QPoint(0, 100);
            translate = QPoint(_length, 0);
            break;
    }
    sortPins();
    for (PinClassItem *pinItem : _pins)
    {
        pinItem->pin()->setAngle(angle);
        pinItem->pin()->setPos(pinPos + translate);
        pinItem->pin()->setLength(_length);
        if (visibilityValue() != VisibilityVisible)
        {
            pinItem->pin()->setPinType(Pin::NotVisible);
        }
        pinPos += offset;
    }
}

QPoint PinClass::pos() const
{
    return _pos;
}

const QRect &PinClass::boundingRect() const
{
    if (!_brect)
    {
        computeBoundingRect();
    }
    return _boundingRect;
}

void PinClass::computeBoundingRect() const
{
    QRect rect;
    KicadFont kicadFont;
    int maxLength = 0;
    for (PinClassItem *pinItem : _pins)
    {
        int width = kicadFont.textWidth(pinItem->pin()->name());
        if (width > maxLength)
        {
            maxLength = width;
        }
    }

    switch (_position)
    {
        case ClassRule::PositionTop:
        case ClassRule::PositionBottom:
            rect = QRect(0, 0, _pins.size() * 100, qCeil((maxLength + 30) / 100.0) * 100);
            if (hasLabelSet() && !_label.isEmpty())
            {
                rect.adjust(0, 0, 0, 100);
            }
            break;
        case ClassRule::PositionLeft:
        case ClassRule::PositionRight:
        case ClassRule::PositionASide:
            rect = QRect(0, 0, qCeil((maxLength + 30) / 100.0) * 100, _pins.size() * 100);
            if (hasLabelSet() && !_label.isEmpty())
            {
                rect.adjust(0, 0, 100, 0);
            }
            break;
    }
    _boundingRect = rect;
    _brect = true;
}

void PinClass::addPinItem(PinClassItem *pin)
{
    _pins.append(pin);
    _brect = false;
}

const QList<PinClassItem *> &PinClass::pins() const
{
    return _pins;
}

DrawText *PinClass::getDrawText() const
{
    if (!hasLabelSet() || _position == ClassRule::PositionASide)
    {
        return nullptr;
    }

    DrawText *drawClassLabel = new DrawText(label());

    int width;
    int height;

    switch (_position)
    {
        case ClassRule::PositionTop:
            height = qCeil(boundingRect().height() / 100.0) * 100 - 100;
            drawClassLabel->setPos(QPoint(_pos.x() + boundingRect().width() / 2 - 50, _pos.y() + 50 + height));
            drawClassLabel->setDirection(DrawText::DirectionHorizontal);
            break;
        case ClassRule::PositionBottom:
            height = qCeil(boundingRect().height() / 100.0) * 100 - 100;
            drawClassLabel->setPos(QPoint(_pos.x() + boundingRect().width() / 2 - 50, _pos.y() - 50 - height));
            drawClassLabel->setDirection(DrawText::DirectionHorizontal);
            break;
        case ClassRule::PositionLeft:
            width = qCeil(boundingRect().width() / 100.0) * 100 - 100;
            drawClassLabel->setPos(QPoint(_pos.x() + width + 50, _pos.y() - 50 + boundingRect().height() / 2));
            drawClassLabel->setDirection(DrawText::DirectionVertital);
            break;
        case ClassRule::PositionRight:
            width = qCeil(boundingRect().width() / 100.0) * 100 - 100;
            drawClassLabel->setPos(QPoint(_pos.x() - width - 50, _pos.y() - 50 + boundingRect().height() / 2));
            drawClassLabel->setDirection(DrawText::DirectionVertital);
            break;
        case ClassRule::PositionASide:
            break;
    }
    drawClassLabel->setTextStyle(DrawText::TextBold);
    drawClassLabel->setTextHJustify(DrawText::TextHCenter);

    return drawClassLabel;
}

DrawRect *PinClass::getDrawRect() const
{
    if (!hasRectSet() || _rect <= 0)
    {
        return nullptr;
    }

    DrawRect *drawRect = new DrawRect();

    drawRect->setThickness(_rect / 0.254);

    int width;
    int height;

    switch (_position)
    {
        case ClassRule::PositionTop:
            drawRect->setPos(QPoint(_pos.x() - 100, _pos.y()));
            height = qCeil(boundingRect().height() / 100.0) * 100;
            if (hasLabelSet() && !_label.isEmpty())
            {
                height -= 100;
            }
            drawRect->setEndPos(QPoint(_pos.x() + boundingRect().width(), _pos.y() + height));
            break;
        case ClassRule::PositionBottom:
            drawRect->setPos(QPoint(_pos.x() - 100, _pos.y()));
            height = qCeil(boundingRect().height() / 100.0) * 100;
            if (hasLabelSet() && !_label.isEmpty())
            {
                height -= 100;
            }
            drawRect->setEndPos(QPoint(_pos.x() + boundingRect().width(), _pos.y() - height));
            break;
        case ClassRule::PositionLeft:
            drawRect->setPos(QPoint(_pos.x(), _pos.y() - 100));
            width = qCeil(boundingRect().width() / 100.0) * 100;
            if (hasLabelSet() && !_label.isEmpty())
            {
                width -= 100;
            }
            drawRect->setEndPos(QPoint(_pos.x() + width, _pos.y() + boundingRect().height()));
            break;
        case ClassRule::PositionRight:
            drawRect->setPos(QPoint(_pos.x(), _pos.y() - 100));
            width = qCeil(boundingRect().width() / 100.0) * 100;
            if (hasLabelSet() && !_label.isEmpty())
            {
                width -= 100;
            }
            drawRect->setEndPos(QPoint(_pos.x() - width, _pos.y() + boundingRect().height()));
            break;
        case ClassRule::PositionASide:
            break;
    }

    return drawRect;
}
