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

PinClass::PinClass(const QString &className)
    : _className(className)
{
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
            if (captures.count()>1)
                newSortPatern.replace("\\1", captures[1]);
            if (captures.count()>2)
                newSortPatern.replace("\\2", captures[2]);
            if (captures.count()>3)
                newSortPatern.replace("\\3", captures[3]);
        }
        setSortPattern(newSortPatern);
    }
    if (!_titleSet && rule->hasTitleSet())
    {
        QString newTitle = rule->title();
        QRegularExpressionMatch match = rule->selector().match(_className, 0, QRegularExpression::NormalMatch);
        if (match.hasMatch())
        {
            const QStringList &captures = (match.capturedTexts());
            if (captures.count()>1)
                newTitle.replace("\\1", captures[1]);
            if (captures.count()>2)
                newTitle.replace("\\2", captures[2]);
            if (captures.count()>3)
                newTitle.replace("\\3", captures[3]);
        }
        setTitle(newTitle);
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

QRect PinClass::boundingRect() const
{
    QRect rect;
    int maxLenght = 0;
    foreach (PinClassItem *pinItem, _pins)
    {
        if (pinItem->pin()->name().size() > maxLenght)
            maxLenght = pinItem->pin()->name().size();
    }

    switch (_position)
    {
    case ClassRule::PositionTop:
    case ClassRule::PositionBottom:
        rect = QRect(0, 0, _pins.size() * 100, maxLenght*50);
        break;
    case ClassRule::PositionLeft:
    case ClassRule::PositionRight:
    case ClassRule::PositionASide:
        rect = QRect(0, 0, maxLenght*50, _pins.size() * 100);
        break;
    }
    return rect;
}

void PinClass::addPinItem(PinClassItem *pin)
{
    _pins.append(pin);
}

const QList<PinClassItem *> &PinClass::pins() const
{
    return _pins;
}

DrawText *PinClass::getDrawText() const
{
    if (!hasTitleSet() || _position == ClassRule::PositionASide)
        return Q_NULLPTR;

    DrawText *drawClassTitle = new DrawText(title());

    switch (_position)
    {
    case ClassRule::PositionTop:
        drawClassTitle->setPos(QPoint(_pos.x() + boundingRect().width() / 2 - 50, _pos.y() + 100 + boundingRect().height()));
        drawClassTitle->setDirection(DrawText::DirectionHorizontal);
        break;
    case ClassRule::PositionBottom:
        drawClassTitle->setPos(QPoint(_pos.x() + boundingRect().width() / 2 - 50, _pos.y() - 100 - boundingRect().height()));
        drawClassTitle->setDirection(DrawText::DirectionHorizontal);
        break;
    case ClassRule::PositionLeft:
        drawClassTitle->setPos(QPoint(_pos.x() + boundingRect().width() + 50, _pos.y() - 50 + boundingRect().height() / 2));
        drawClassTitle->setDirection(DrawText::DirectionVertital);
        break;
    case ClassRule::PositionRight:
        drawClassTitle->setPos(QPoint(_pos.x() - boundingRect().width() - 50, _pos.y() - 50 + boundingRect().height() / 2));
        drawClassTitle->setDirection(DrawText::DirectionVertital);
        break;
    case ClassRule::PositionASide:
        break;
    }
    drawClassTitle->setTextStyle(DrawText::TextBold);
    drawClassTitle->setTextHJustify(DrawText::TextHCenter);

    return drawClassTitle;
}

DrawRect *PinClass::getDrawRect() const
{
    if (!hasRectSet() || _rect <= 0)
        return Q_NULLPTR;

    DrawRect *drawRect = new DrawRect();

    drawRect->setThickness(_rect / 0.254);

    switch (_position)
    {
    case ClassRule::PositionTop:
        drawRect->setPos(QPoint(_pos.x() - 100, _pos.y()));
        drawRect->setEndPos(QPoint(_pos.x() + boundingRect().width(), _pos.y() + boundingRect().height() + 50));
        break;
    case ClassRule::PositionBottom:
        drawRect->setPos(QPoint(_pos.x() - 100, _pos.y()));
        drawRect->setEndPos(QPoint(_pos.x() + boundingRect().width(), _pos.y() - boundingRect().height() - 50));
        break;
    case ClassRule::PositionLeft:
        drawRect->setPos(QPoint(_pos.x(), _pos.y() - 100));
        drawRect->setEndPos(QPoint(_pos.x() + boundingRect().width(), _pos.y() + boundingRect().height()));
        break;
    case ClassRule::PositionRight:
        drawRect->setPos(QPoint(_pos.x(), _pos.y() - 100));
        drawRect->setEndPos(QPoint(_pos.x() - boundingRect().width(), _pos.y() + boundingRect().height()));
        break;
    case ClassRule::PositionASide:
        break;
    }

    return drawRect;
}
