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

#include "component.h"
#include "drawrect.h"

#include <QDebug>
#include <QFontMetrics>

#include <qmath.h>

/**
 * @brief Base constructor for component
 * @param name optionally specify the component name at the creation
 */
Component::Component(const QString &name)
    : _prefix("U")
{
    _showPinName = true;
    _showPadName = true;
    _unitCount = 1;
    _refText = new DrawText("U");
    _nameText = new DrawText();
    _packageText = new DrawText();
    _packageText->setVisible(false);
    _docText = new DrawText();
    _docText->setVisible(false);
    setName(name);
}

/**
 * @brief Component copy constructor. Also copy all the pins.
 * @param other component to copy
 */
Component::Component(const Component &other)
{
    _name = other._name;
    _prefix = other._prefix;
    _alias = other._alias;
    _footPrints = other._footPrints;
    _showPinName = other._showPinName;
    _showPadName = other._showPadName;
    _unitCount = other._unitCount;

    for (auto pin : other._pins)
    {
        addPin(new Pin(*pin));
    }
    for (auto draw : other._draws)
    {
        addDraw(draw->clone());
    }

    _refText = new DrawText(*other._refText);
    _nameText = new DrawText(*other._nameText);
    _packageText = new DrawText(*other._packageText);
    _docText = new DrawText(*other._nameText);
}

/**
 * @brief Component destructor. Delete also all the internal pins.
 */
Component::~Component()
{
    for (auto &pin : _pins)
    {
        delete pin;
    }
    for (auto &draw : _draws)
    {
        delete draw;
    }
    delete _refText;
    delete _nameText;
    delete _packageText;
    delete _docText;
}

/**
 * @brief Component name getter.
 * @return component name
 */
const QString &Component::name() const
{
    return _name;
}

/**
 * @brief Component name setter.
 * @param name name to give
 * @warning replace all the space contained in the name by underscore
 */
void Component::setName(const QString &name)
{
    _name = name;
    _name.replace(" ", "_");
    _nameText->setText(name);
}

/**
 * @brief Component pins list getter
 * @return list of pins
 */
QList<Pin *> &Component::pins()
{
    return _pins;
}

/**
 * @brief Constant component pins list getter
 * @return list of pins
 */
const QList<Pin *> &Component::pins() const
{
    return _pins;
}

/**
 * @brief Adds a pin to the component
 * @param pin pin pointer to add
 */
void Component::addPin(Pin *pin)
{
    pin->setComponent(this);
    _pins.append(pin);
}

/**
 * @brief Removes a pin from his pointer
 * @param pin pin pointer to remove
 */
void Component::removePin(Pin *pin)
{
    if (_pins.removeOne(pin))
    {
        delete pin;
    }
}

/**
 * @brief Removes and deletes all pins
 */
void Component::clearPins()
{
    for (auto &pin : _pins)
    {
        delete pin;
    }
    _pins.clear();
}

/**
 * @brief Component draw items list getter
 * @return list of draws
 */
QList<Draw *> &Component::draws()
{
    return _draws;
}

/**
 * @brief Constant component draw items list getter
 * @return list of draws
 */
const QList<Draw *> &Component::draws() const
{
    return _draws;
}

/**
 * @brief Adds a item draw to the component at the first position
 * @param draw draw pointer to add
 */
void Component::prependDraw(Draw *draw)
{
    draw->setComponent(this);
    _draws.prepend(draw);
}

/**
 * @brief Adds a item draw to the component
 * @param draw draw pointer to add
 */
void Component::addDraw(Draw *draw)
{
    draw->setComponent(this);
    _draws.append(draw);
}

/**
 * @brief Removes a draw item from his pointer
 * @param draw draw pointer to remove
 */
void Component::removeDraw(Draw *draw)
{
    if (_draws.removeOne(draw))
    {
        delete draw;
    }
}

/**
 * @brief Removes and deletes all draws
 */
void Component::clearDraws()
{
    for (auto &draw : _draws)
    {
        delete draw;
    }
    _draws.clear();
}

/**
 * @brief Component prefix letter(s) getter
 * @return prefix letter(s)
 */
const QString &Component::prefix() const
{
    return _prefix;
}

/**
 * @brief Sets one or more letters prefix for the component. For ex: "U", "IC"
 * @param prefix prefix letter(s)
 */
void Component::setPrefix(const QString &prefix)
{
    _prefix = prefix;
}

/**
 * @brief Returns a reference to the list of component aliases name for identical pin package configuration
 * @return aliases list
 */
QStringList &Component::aliases()
{
    return _alias;
}

/**
 * @brief Returns the list of component aliases name for identical pin package configuration
 * @return aliases list
 */
const QStringList &Component::aliases() const
{
    return _alias;
}

/**
 * @brief Adds an alias name to the component
 * @param alias alias name to add
 */
void Component::addAlias(const QString &alias)
{
    _alias.append(alias);
}

/**
 * @brief Adds an alias list name to the component
 * @param aliases aliases names to add
 */
void Component::addAlias(const QStringList &aliases)
{
    _alias.append(aliases);
}

/**
 * @brief Returns a reference to possible footprint list
 * @return non const footprint list
 */
QStringList &Component::footPrints()
{
    return _footPrints;
}

/**
 * @brief Returns possible footprint list
 * @return footprint list
 */
const QStringList &Component::footPrints() const
{
    return _footPrints;
}

/**
 * @brief Adds a footprint name to the list of possible footprints for this component
 * @param footprint name to add
 */
void Component::addFootPrint(const QString &footprint)
{
    _footPrints.append(footprint);
}

/**
 * @brief Adds a footprint list to the list of possible footprints for this component
 * @param footprint list to add
 */
void Component::addFootPrint(const QStringList &footprints)
{
    _footPrints.append(footprints);
}

/**
 * @brief Returns the bounding rect of component with pin lenght
 * @return bounding rect
 */
QRect Component::boundingRect() const
{
    QRect mrect(0, 0, 1, 1);
    for (auto pin : _pins)
    {
        mrect = mrect.united(QRect(pin->pos(), QSize(1, 1)));
    }
    return mrect;
}

/**
 * @brief Component rect
 * @return component rect
 */
QRect Component::rect() const
{
    return _rect;
}

/**
 * @brief Sets rect of component
 * @param rect component rect
 */
void Component::setRect(const QRect &rect)
{
    _rect = rect;
}

bool pinPadLessThan(const Pin *pin1, const Pin *pin2)
{
    return (pin1->padName().rightJustified(4, '0') < pin2->padName().rightJustified(4, '0'));
}

/**
 * @brief Sorts the component by pad name
 */
void Component::sort()
{
    std::sort(_pins.begin(), _pins.end(), pinPadLessThan);
}

/**
 * @brief Pin name is shown if true
 * @return true if pin name is shown
 */
bool Component::showPinName() const
{
    return _showPinName;
}

/**
 * @brief Makes pin name visible
 * @param showPinName Pin name is shown if true
 */
void Component::setShowPinName(bool showPinName)
{
    _showPinName = showPinName;
}

/**
 * @brief Pad name is shown if true
 * @return true if pad name is shown
 */
bool Component::showPadName() const
{
    return _showPadName;
}

/**
 * @brief Makes pad name visible
 * @param showPadName Pad name is shown if true
 */
void Component::setShowPadName(bool showPadName)
{
    _showPadName = showPadName;
}

/**
 * @brief Returns the number of units declared in the component
 * @return unit count
 */
int Component::unitCount() const
{
    return _unitCount;
}

/**
 * @brief Sets the number of units declared in the component
 * @param unitCount Count of units
 */
void Component::setUnitCount(int unitCount)
{
    _unitCount = unitCount;
}

/**
 * @brief Image of debugger for extraction from PDF
 * @return image view of PDF with color rects
 */
const QImage &Component::debugInfo() const
{
    return _debugInfo;
}

/**
 * @brief Sets the debug image view
 * @param debugInfo debug image view
 */
void Component::setDebugInfo(const QImage &debugInfo)
{
    _debugInfo = debugInfo;
}

/**
 * @brief Reference draw text
 * @return Pointer to reference draw text
 */
DrawText *Component::refText() const
{
    return _refText;
}

/**
 * @brief Sets reference draw text
 * @param refText Pointer to reference draw text
 */
void Component::setRefText(DrawText *refText)
{
    delete _refText;
    _refText = refText;
}

/**
 * @brief Reference name text
 * @return Pointer to name draw text
 */
DrawText *Component::nameText() const
{
    return _nameText;
}

/**
 * @brief Sets name draw text
 * @param refText Pointer to name draw text
 */
void Component::setNameText(DrawText *nameText)
{
    delete _nameText;
    _nameText = nameText;
}

/**
 * @brief Reference package text
 * @return Pointer to package draw text
 */
DrawText *Component::packageText() const
{
    return _packageText;
}

/**
 * @brief Sets package draw text
 * @param refText Pointer to package draw text
 */
void Component::setPackageText(DrawText *packageText)
{
    delete _packageText;
    _packageText = packageText;
}

/**
 * @brief Reference doc text
 * @return Pointer to doc draw text
 */
DrawText *Component::docText() const
{
    return _docText;
}

/**
 * @brief Sets doc draw text
 * @param refText Pointer to doc draw text
 */
void Component::setDocText(DrawText *docText)
{
    delete _docText;
    _docText = docText;
}

/**
 * @brief Reorganizes component pins like a simple DIP/SOIC package
 */
void Component::reorganizeToPackageStyle()
{
    QPoint pos;
    int i;

    int rightCount = _pins.count() / 2;
    int leftCount = _pins.count() - rightCount;

    int leftOffset = (static_cast<int>(ceil(leftCount / 2.0)) - 1) * 100;
    int rightOffset = (static_cast<int>(ceil(rightCount / 2.0)) - 1) * 100;
    if (leftCount % 2 == 0)
    {
        leftOffset += 100;
    }

    int margin = 0;
    int leftMargin = 0;
    int rightMargin = 0;
    sort();
    // compute leftMargin and rightMargin
    for (i = 0; i < leftCount; i++)
    {
        Pin *pin = _pins[i];
        pin->setPinType(Pin::Normal);
        pin->setLength(200);
        int width = pin->name().count() * 50 + 250;
        if (width > leftMargin)
        {
            leftMargin = width;
        }
    }
    for (; i < leftCount + rightCount; i++)
    {
        Pin *pin = _pins[i];
        pin->setPinType(Pin::Normal);
        pin->setLength(200);
        int width = pin->name().count() * 50 + 250;
        if (width > rightMargin)
        {
            rightMargin = width;
        }
    }
    margin = (leftMargin + rightMargin) / 2;
    margin = (qCeil(margin / 100.0)) * 100;  // grid align

    // place pins
    pos = QPoint(-margin, -leftOffset);
    for (i = 0; i < leftCount; i++)
    {
        Pin *pin = _pins[i];

        pin->setAngle(0);
        pin->setPos(pos);
        pos += QPoint(0, 100);
    }
    pos = QPoint(margin, rightOffset);
    for (; i < leftCount + rightCount; i++)
    {
        Pin *pin = _pins[i];

        pin->setAngle(180);
        pin->setPos(pos);
        pos += QPoint(0, -100);
    }

    clearDraws();
    // added draw outside rect
    QRect rect = QRect(QPoint(-margin + 200, -qMax(leftOffset, rightOffset) - 100), QPoint(margin - 200, qMax(leftOffset, rightOffset) + 100));
    addDraw(new DrawRect(rect));

    // set position of ref and name text
    _nameText->setPos(QPoint(rect.right(), rect.top() - 50));
    _nameText->setTextHJustify(DrawText::TextHRight);
    _nameText->setDirection(DrawText::DirectionHorizontal);
    _refText->setPos(QPoint(rect.left(), rect.bottom() + 50));
    _refText->setTextHJustify(DrawText::TextHLeft);
    _refText->setDirection(DrawText::DirectionHorizontal);
}
