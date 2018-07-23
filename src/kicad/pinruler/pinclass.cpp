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

#include "pinclass.h"

#include <QDebug>
#include <qmath.h>

#include "viewer/kicadfont.h"

PinClass::PinClass(const QString &className)
    : _className(className)
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
        setPosition(rule->positionValue());
    if (!_sortSet && rule->hasSortSet())
        setSort(rule->sortValue());
    if (!_sortPatternSet && rule->hasSortPatternSet())
    {
        QString newSortPatern = rule->sortPattern();
        QRegularExpressionMatch match = rule->selector().match(_className, 0, QRegularExpression::NormalMatch);
        if (match.hasMatch())
        {
            const QStringList &captures = (match.capturedTexts());
            if (captures.count() > 1)
                newSortPatern.replace("\\1", captures[1]);
            if (captures.count() > 2)
                newSortPatern.replace("\\2", captures[2]);
            if (captures.count() > 3)
                newSortPatern.replace("\\3", captures[3]);
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
                newLabel.replace("\\1", captures[1]);
            if (captures.count() > 2)
                newLabel.replace("\\2", captures[2]);
            if (captures.count() > 3)
                newLabel.replace("\\3", captures[3]);
        }
        setLabel(newLabel);
    }
    if (!_lengthSet && rule->hasLengthSet())
        setLength(rule->length());
    if (!_prioritySet && rule->hasPrioritySet())
        setPriority(rule->priority());
    if (!_visibilitySet && rule->hasVisibilitySet())
        setVisibility(rule->visibilityValue());
    if (!_rectSet && rule->hasRectSet())
        setRect(rule->rect());
}

void PinClass::applyRules(QList<ClassRule *> rules)
{
    foreach (ClassRule *rule, rules)
        applyRule(rule);
}

void PinClass::sortPins()
{
    if (_sort == ClassRule::SortNone)
        return;

    QRegularExpression pattern(_sortPattern, QRegularExpression::CaseInsensitiveOption);
    QRegularExpression numPattern("([^0-9]*)([0-9]+)([^0-9]*)", QRegularExpression::CaseInsensitiveOption);

    foreach (PinClassItem *pinItem, _pins)
    {
        QString sortPatern;
        QString pinName = pinItem->pin()->name();
        pinName.replace("~", "");

        if (pattern.isValid())
        {
            QRegularExpressionMatch match = pattern.match(pinName);
            if (match.hasMatch())
                sortPatern = match.captured(0);
            else
                sortPatern = pinName;
        }
        else
            sortPatern = pinName;
        //sortPatern = sortPatern.leftJustified(n, ' ');

        QRegularExpressionMatchIterator numMatchIt = numPattern.globalMatch(sortPatern);
        if (numMatchIt.hasNext())
            sortPatern.clear();
        while (numMatchIt.hasNext())
        {
            QRegularExpressionMatch numMatch = numMatchIt.next();
            sortPatern.append(numMatch.captured(1) + QString('0').repeated(5-numMatch.captured(2).size()) + numMatch.captured(2) + numMatch.captured(3));
        }
        QRegularExpressionMatchIterator padMatchIt = numPattern.globalMatch(pinItem->pin()->padName());
        while (padMatchIt.hasNext())
        {
            QRegularExpressionMatch padMatch = padMatchIt.next();
            sortPatern.append(padMatch.captured(1) + QString('0').repeated(5-padMatch.captured(2).size()) + padMatch.captured(2) + padMatch.captured(3));
        }

        pinItem->setSortLabel(sortPatern);
    }

    if (_sort == ClassRule::SortAsc)
        qSort(_pins.begin(), _pins.end(), PinClassItem::pinLessThan);
    if (_sort == ClassRule::SortDesc)
        qSort(_pins.begin(), _pins.end(), PinClassItem::pinGreaterThan);

}

void PinClass::setPos(const QPoint &basePos)
{
    _pos = basePos;

    QPoint pinPos = basePos;
    QPoint offset;
    QPoint translate;
    Pin::Direction direction;

    switch (_position)
    {
    case ClassRule::PositionTop:
        direction = Pin::Down;
        offset = QPoint(100, 0);
        translate = QPoint(0, -_length);
        break;
    case ClassRule::PositionBottom:
        direction = Pin::Up;
        offset = QPoint(100, 0);
        translate = QPoint(0, _length);
        break;
    case ClassRule::PositionLeft:
        direction = Pin::Right;
        offset = QPoint(0, 100);
        translate = QPoint(-_length, 0);
        break;
    case ClassRule::PositionRight:
    case ClassRule::PositionASide:
        direction = Pin::Left;
        offset = QPoint(0, 100);
        translate = QPoint(_length, 0);
        break;
    }
    sortPins();
    foreach (PinClassItem *pinItem, _pins)
    {
        pinItem->pin()->setDirection(direction);
        pinItem->pin()->setPos(pinPos + translate);
        pinItem->pin()->setLength(_length);
        if (visibilityValue() != VisibilityVisible)
            pinItem->pin()->setPinType(Pin::NotVisible);
        pinPos += offset;
    }
}

QPoint PinClass::getPos() const
{
    return _pos;
}

const QRect &PinClass::boundingRect() const
{
    if (!_brect)
        computeBoundingRect();
    return _boundingRect;
}

void PinClass::computeBoundingRect() const
{
    QRect rect;
    KicadFont kicadFont;
    int maxWidth = 0;
    foreach (PinClassItem *pinItem, _pins)
    {
        int width = kicadFont.textWidth(pinItem->pin()->name());
        if (width > maxWidth)
            maxWidth = width;
    }

    switch (_position)
    {
    case ClassRule::PositionTop:
    case ClassRule::PositionBottom:
        rect = QRect(0, 0, _pins.size() * 100, maxWidth + 20);
        break;
    case ClassRule::PositionLeft:
    case ClassRule::PositionRight:
    case ClassRule::PositionASide:
        rect = QRect(0, 0, maxWidth + 20, _pins.size() * 100);
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
        return Q_NULLPTR;

    DrawText *drawClassLabel = new DrawText(label());

    int width, height;

    switch (_position)
    {
    case ClassRule::PositionTop:
        height = qCeil(boundingRect().height() / 100.0) * 100;
        drawClassLabel->setPos(QPoint(_pos.x() + boundingRect().width() / 2 - 50, _pos.y() + 50 + height));
        drawClassLabel->setDirection(DrawText::DirectionHorizontal);
        break;
    case ClassRule::PositionBottom:
        height = qCeil(boundingRect().height() / 100.0) * 100;
        drawClassLabel->setPos(QPoint(_pos.x() + boundingRect().width() / 2 - 50, _pos.y() - 50 - height));
        drawClassLabel->setDirection(DrawText::DirectionHorizontal);
        break;
    case ClassRule::PositionLeft:
        width = qCeil(boundingRect().width() / 100.0) * 100;
        drawClassLabel->setPos(QPoint(_pos.x() + width + 50, _pos.y() - 50 + boundingRect().height() / 2));
        drawClassLabel->setDirection(DrawText::DirectionVertital);
        break;
    case ClassRule::PositionRight:
        width = qCeil(boundingRect().width() / 100.0) * 100;
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
        return Q_NULLPTR;

    DrawRect *drawRect = new DrawRect();

    drawRect->setThickness(_rect / 0.254);

    int width, height;

    switch (_position)
    {
    case ClassRule::PositionTop:
        drawRect->setPos(QPoint(_pos.x() - 100, _pos.y()));
        height = qCeil(boundingRect().height() / 100.0) * 100;
        drawRect->setEndPos(QPoint(_pos.x() + boundingRect().width(), _pos.y() + height));
        break;
    case ClassRule::PositionBottom:
        drawRect->setPos(QPoint(_pos.x() - 100, _pos.y()));
        height = qCeil(boundingRect().height() / 100.0) * 100;
        drawRect->setEndPos(QPoint(_pos.x() + boundingRect().width(), _pos.y() - height));
        break;
    case ClassRule::PositionLeft:
        drawRect->setPos(QPoint(_pos.x(), _pos.y() - 100));
        width = qCeil(boundingRect().width() / 100.0) * 100;
        drawRect->setEndPos(QPoint(_pos.x() + width, _pos.y() + boundingRect().height()));
        break;
    case ClassRule::PositionRight:
        drawRect->setPos(QPoint(_pos.x(), _pos.y() - 100));
        width = qCeil(boundingRect().width() / 100.0) * 100;
        drawRect->setEndPos(QPoint(_pos.x() - width, _pos.y() + boundingRect().height()));
        break;
    case ClassRule::PositionASide:
        break;
    }

    return drawRect;
}
