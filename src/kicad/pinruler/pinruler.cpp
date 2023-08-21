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

#include "pinruler.h"

#include <QDebug>
#include <qmath.h>

#include "model/drawrect.h"

PinRuler::PinRuler(RulesSet *ruleSet)
{
    _ruleSet = ruleSet;
}

bool nameGreaterThan(PinClass *c1, PinClass *c2)
{
    return c1->className() < c2->className();
}

bool heightGreaterThan(PinClass *c1, PinClass *c2)
{
    if (c1->boundingRect().height() == c2->boundingRect().height())
    {
        return nameGreaterThan(c1, c2);
    }
    return c1->boundingRect().height() > c2->boundingRect().height();
}

bool prioGreaterThan(PinClass *c1, PinClass *c2)
{
    if (c1->priority() == c2->priority())
    {
        return nameGreaterThan(c1, c2);
    }
    return c1->priority() > c2->priority();
}

void PinRuler::organize(Component *component)
{
    int x;
    int y;

    component->clearDraws();

    PinClass *defaultClass = pinClass("default");
    for (Pin *pin : component->pins())
    {
        PinClassItem *pinClassItem = new PinClassItem(pin);
        const QList<PinRule *> &rules = _ruleSet->rulesForPin(pin->name());
        if (rules.isEmpty())
        {
            pin->setPinType(Pin::NotVisible);
            defaultClass->addPinItem(pinClassItem);
        }
        else
        {
            pin->setUnit(1);
            QString className;  // = rules.first()->className(pin->name());
            for (PinRule *rule : rules)
            {
                if (rule->hasClassSet())
                {
                    className = rule->className(pin->name());
                    break;
                }
            }
            PinClass *mpinClass = pinClass(className);
            for (PinRule *rule : rules)
            {
                if (rule->hasElecTypeSet())
                {
                    pin->setElectricalType(rule->elecType());
                    break;
                }
            }
            pin->setPinType(Pin::Normal);
            for (PinRule *rule : rules)
            {
                if (rule->hasPinTypeSet())
                {
                    pin->setPinType(rule->pinType());
                    break;
                }
            }
            for (PinRule *rule : rules)
            {
                if (rule->hasPrioritySet())
                {
                    pinClassItem->setPriority(rule->priority());
                    break;
                }
            }
            if (mpinClass->label().isEmpty())
            {
                pin->setClassName(mpinClass->className());
            }
            else
            {
                pin->setClassName(mpinClass->label());
            }

            mpinClass->addPinItem(pinClassItem);
        }
    }

    // sideSize computation and side class list assignment
    QList<PinClass *> topSide;
    QList<PinClass *> bottomSide;
    QList<PinClass *> leftSide;
    QList<PinClass *> rightSide;
    QList<PinClass *> aSide;        // last assign
    QList<PinClass *> removedPins;  // last assign
    QSize topSize = QSize(0, 0);
    QSize bottomSize = QSize(0, 0);
    QSize leftSize = QSize(0, 0);
    QSize rightSize = QSize(0, 0);
    QSize removedSize = QSize(0, 0);
    for (PinClass *mpinClass : _pinClasses)
    {
        if (mpinClass->pins().count() == 0)
        {
            continue;
        }

        QRect rect = mpinClass->boundingRect();
        if (mpinClass->visibilityValue() == ClassRule::VisibilityRemoved)
        {
            mpinClass->setPosition(PinClass::PositionLeft);
            removedPins.append(mpinClass);
            removedSize.rwidth() += rect.width();
            if (rect.height() > removedSize.height())
            {
                removedSize.setHeight(rect.height());
            }
            continue;
        }

        switch (mpinClass->positionValue())
        {
            case ClassRule::PositionTop:
                topSide.append(mpinClass);
                topSize.rwidth() += rect.width();
                if (rect.height() > topSize.height())
                {
                    topSize.setHeight(rect.height());
                }
                break;
            case ClassRule::PositionBottom:
                bottomSide.append(mpinClass);
                bottomSize.rwidth() += rect.width();
                if (rect.height() > bottomSize.height())
                {
                    bottomSize.setHeight(rect.height());
                }
                break;
            case ClassRule::PositionLeft:
                leftSide.append(mpinClass);
                leftSize.rheight() += rect.height();
                if (rect.width() > leftSize.width())
                {
                    leftSize.setWidth(rect.width());
                }
                break;
            case ClassRule::PositionRight:
                rightSide.append(mpinClass);
                rightSize.rheight() += rect.height();
                if (rect.width() > rightSize.width())
                {
                    rightSize.setWidth(rect.width());
                }
                break;
            case ClassRule::PositionASide:
                aSide.append(mpinClass);
                break;
        }
    }

    // aside list assignment and right and left sides updates
    std::sort(aSide.begin(), aSide.end(), heightGreaterThan);
    for (PinClass *mpinClass : aSide)
    {
        QRect rect = mpinClass->boundingRect();
        if (leftSize.height() < rightSize.height())
        {
            mpinClass->setPosition(ClassRule::PositionLeft);
            leftSide.append(mpinClass);
            leftSize.rheight() += rect.height();
            if (rect.width() > leftSize.width())
            {
                leftSize.setWidth(rect.width());
            }
        }
        else
        {
            mpinClass->setPosition(ClassRule::PositionRight);
            rightSide.append(mpinClass);
            rightSize.rheight() += rect.height();
            if (rect.width() > rightSize.width())
            {
                rightSize.setWidth(rect.width());
            }
        }
    }

    std::sort(leftSide.begin(), leftSide.end(), prioGreaterThan);
    std::sort(rightSide.begin(), rightSide.end(), prioGreaterThan);
    std::sort(topSide.begin(), topSide.end(), prioGreaterThan);
    std::sort(bottomSide.begin(), bottomSide.end(), prioGreaterThan);

    // margins
    leftSize.rheight() += (leftSide.size() - 1) * 100;
    rightSize.rheight() += (rightSide.size() - 1) * 100;

    // placement
    int sideX = qMax((leftSize.width() + rightSize.width()) / 2, qMax(topSize.width(), bottomSize.width()) / 2);
    sideX = qCeil(sideX / 100.0) * 100;  // grid align KLC4.1
    int sideY = (qMax(leftSize.height(), rightSize.height()) + (topSize.height() + bottomSize.height() + 50)) / 2;
    sideY = qCeil(sideY / 100.0) * 100;  // grid align KLC4.1

    x = -sideX;
    y = -sideY + qCeil(topSize.height() / 100.0) * 100 + 100;
    if (leftSize.height() + 50 < rightSize.height())
    {
        y += qCeil((rightSize.height() - leftSize.height()) / 200.0) * 100;  // grid align KLC4.1
    }
    for (PinClass *mpinClass : leftSide)
    {
        mpinClass->setPos(QPoint(x, y));
        if (mpinClass->hasLabelSet())
        {
            component->addDraw(mpinClass->getDrawText());
        }
        if (mpinClass->hasRectSet() && static_cast<int>(mpinClass->rect()) > 0)
        {
            component->addDraw(mpinClass->getDrawRect());
        }

        y += mpinClass->boundingRect().height() + 100;
    }

    x = sideX;
    y = -sideY + qCeil(topSize.height() / 100.0) * 100 + 100;
    if (rightSize.height() + 50 < leftSize.height())
    {
        y += qCeil((leftSize.height() - rightSize.height()) / 200.0) * 100;  // grid align KLC4.1
    }
    for (PinClass *mpinClass : rightSide)
    {
        mpinClass->setPos(QPoint(x, y));
        if (mpinClass->hasLabelSet())
        {
            component->addDraw(mpinClass->getDrawText());
        }
        if (mpinClass->hasRectSet() && static_cast<int>(mpinClass->rect()) > 0)
        {
            component->addDraw(mpinClass->getDrawRect());
        }

        y += mpinClass->boundingRect().height() + 100;
    }

    x = -topSize.width() / 2;
    x = qCeil(x / 100.0) * 100;  // grid align KLC4.1
    y = -sideY;
    for (PinClass *mpinClass : topSide)
    {
        mpinClass->setPos(QPoint(x, y));
        if (mpinClass->hasLabelSet())
        {
            component->addDraw(mpinClass->getDrawText());
        }
        if (mpinClass->hasRectSet() && static_cast<int>(mpinClass->rect()) > 0)
        {
            component->addDraw(mpinClass->getDrawRect());
        }

        x += mpinClass->boundingRect().width() + 100;
    }

    x = -bottomSize.width() / 2;
    x = qCeil(x / 100.0) * 100;  // grid align KLC4.1
    y = sideY;
    for (PinClass *mpinClass : bottomSide)
    {
        mpinClass->setPos(QPoint(x, y));
        if (mpinClass->hasLabelSet())
        {
            component->addDraw(mpinClass->getDrawText());
        }
        if (mpinClass->hasRectSet() && static_cast<int>(mpinClass->rect()) > 0)
        {
            component->addDraw(mpinClass->getDrawRect());
        }

        x += mpinClass->boundingRect().width() + 100;
    }

    // TODO : improve me, in case of a large number of removed pins
    // this pseudo class goes outside of the component
    x = -qCeil(removedSize.width() / 200.0) * 100;
    y = -qCeil(removedSize.height() / 200.0) * 100;
    for (PinClass *mpinClass : removedPins)
    {
        mpinClass->setPos(QPoint(x, y));
        y += mpinClass->boundingRect().height() + 100;
    }

    // rect compute
    QRect rect;
    rect.setLeft(-sideX);
    rect.setWidth(sideX * 2 + 1);
    rect.setTop(-sideY);
    rect.setHeight(sideY * 2 + 1);
    DrawRect *rectDraw = new DrawRect(rect);
    rectDraw->setFilled(DrawRect::DrawFilledBackGround);
    component->prependDraw(rectDraw);
    component->nameText()->setPos(QPoint(rect.right(), rect.bottom() + 50));
    component->nameText()->setTextHJustify(DrawText::TextHRight);
    component->nameText()->setDirection(DrawText::DirectionHorizontal);
    component->refText()->setPos(QPoint(rect.left(), rect.bottom() + 50));
    component->refText()->setTextHJustify(DrawText::TextHLeft);
    component->refText()->setDirection(DrawText::DirectionHorizontal);

    for (PinClass *mpinClass : _pinClasses)
    {
        delete mpinClass;
    }
    _pinClasses.clear();
}

RulesSet *PinRuler::ruleSet() const
{
    return _ruleSet;
}

void PinRuler::setRuleSet(RulesSet *ruleSet)
{
    delete _ruleSet;
    _ruleSet = ruleSet;
}

PinClass *PinRuler::addClass(const QString &className)
{
    PinClass *pinClass = new PinClass(className);

    const QList<ClassRule *> &rules = _ruleSet->rulesForClass(className);
    pinClass->applyRules(rules);

    _pinClasses.insert(className, pinClass);
    return pinClass;
}

PinClass *PinRuler::pinClass(const QString &className)
{
    QMap<QString, PinClass *>::const_iterator classFind = _pinClasses.constFind(className);
    if (classFind == _pinClasses.cend())
    {
        return addClass(className);
    }

    return *classFind;
}
