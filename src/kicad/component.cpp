#include "component.h"

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
    setName(name);
    _valid = true;
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
    _rect = other._rect;
    _valid = other._valid;

    for (int i=0; i<other._pins.size(); i++)
        addPin(new Pin(*other._pins[i]));
}

/**
 * @brief Component destructor. Delete also all the internal pins.
 */
Component::~Component()
{
    for (int i=0; i<_pins.size(); i++)
        delete _pins[i];
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
    _name.replace(" ","_");
}

/**
 * @brief Component pins list getter
 * @return list of pins
 */
QList<Pin *> Component::pins()
{
    return _pins;
}

/**
 * @brief Constant component pins list getter
 * @return list of pins
 */
const QList<Pin *> Component::pins() const
{
    return _pins;
}

/**
 * @brief Adds a pin to the component
 * @param pin pin pointer to add
 */
void Component::addPin(Pin *pin)
{
    _pins.append(pin);
}

/**
 * @brief Removes a pin from his pointer
 * @param pin pin pointer to remove
 */
void Component::removePin(Pin *pin)
{
    if (_pins.removeOne(pin))
        delete pin;
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
 * @brief Returns the bounding rect of component without pin lenght
 * @return bounding rect
 */
const QRect &Component::rect() const
{
    return _rect;
}

/**
 * @brief Sets the bounding rect of component
 * @param rect bounding rect
 */
void Component::setRect(const QRect &rect)
{
    _rect = rect;
}

/**
 * @brief Sorts the component by pin name
 */
void Component::sort()
{
    qSort(_pins);
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

    int leftOffset = leftCount * 100 / 2 - 50;
    int rightOffset = rightCount * 100 / 2 - 50;

    int margin = 0;
    int leftMargin = 0, rightMargin = 0;
    // compute leftMargin and rightMargin
    for (i = 0; i < leftCount; i++)
    {
        Pin *pin = _pins[i];
        pin->setPinType(Pin::Normal);
        int width = pin->name().count()*50+350;
        if (width > leftMargin)
            leftMargin = width;
    }
    for (; i < leftCount + rightCount; i++)
    {
        Pin *pin = _pins[i];
        pin->setPinType(Pin::Normal);
        int width = pin->name().count()*50+350;
        if (width > rightMargin)
            rightMargin = width;
    }
    margin = (leftMargin + rightMargin) / 2;
    margin = (qCeil(margin / 100.0)) * 100; // grid align

    // place pins
    pos = QPoint(-margin, -leftOffset);
    for (i = 0; i < leftCount; i++)
    {
        Pin *pin = _pins[i];

        pin->setDirection(Pin::Right);
        pin->setPos(pos);
        pos += QPoint(0, 100);
    }
    pos = QPoint(margin, rightOffset);
    for (; i < leftCount + rightCount; i++)
    {
        Pin *pin = _pins[i];

        pin->setDirection(Pin::Left);
        pin->setPos(pos);
        pos += QPoint(0, -100);
    }

    _rect = QRect(QPoint(-margin+300, -leftOffset-100),
                  QPoint(margin-300, rightOffset+100));
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
 * @brief If a component is correctly parsed and contains good informations
 * @return true if the component is valid
 */
bool Component::isValid() const
{
    return _valid;
}

/**
 * @brief Operator to serialise the component in Kicad format
 * @param stream out stream
 * @param component component to serialise
 * @return stream
 */
QTextStream &operator>>(QTextStream &stream, Component &component)
{
    /*QRegExp regexp("^F([0-9]) \"(\\S*)\" (\\-?[0-9]+) (\\-?[0-9]+) ([0-9]+) "
                   "([A-Z]) ([A-Z]) ([A-Z]) ([A-Z]+)$");*/
    bool draw = false;
    do
    {
        QString start;
        stream >> start;
        if (start.at(0) == '#') // comment
        {
            stream.readLine();
        }
        else if (start == "DEF")
        {
            QString name;
            stream >> name;
            component.setName(name);

            QString prefix;
            stream >> prefix;
            component.setPrefix(prefix);

            stream.readLine();
        }
        else if (start.at(0) == 'F')
        {
            /*qDebug() << regexp.cap(1) << regexp.cap(2) << regexp.cap(3)
                     << regexp.cap(4) << regexp.cap(5);*/
            stream.readLine();
        }
        else if (start =="$FPLIST")
        {
            QString footprint;
            while (!stream.atEnd())
            {
                stream >> footprint;
                if (footprint == "$ENDFPLIST")
                    break;
                component.addFootPrint(footprint);
            }
        }
        else if (start.startsWith("DRAW"))
        {
            draw = true;
            stream.readLine();
        }
        else if (start.startsWith("ALIAS"))
        {
            QString aliases = stream.readLine();
            component._alias.append(aliases.split(' '));
        }
        else if (start.startsWith("ENDDRAW"))
        {
            draw = false;
            stream.readLine();
        }
        else if (start.startsWith("ENDDEF"))
        {
            component._valid = true;
            draw = false;
            stream.readLine();
            return stream;
        }
        else if (draw)
        {
            if (start.at(0) == 'X')
            {
                Pin *pin = new Pin();
                stream >> *pin;
                if (pin->isValid())
                    component.addPin(pin);
                else
                {
                    delete pin;
                    stream.readLine();
                }
            }
            else if (start.startsWith("S"))
            {
                QRect rect;
                int n;
                stream >> n;
                rect.setX(n);
                stream >> n;
                rect.setY(n);
                stream >> n;
                rect.setRight(n);
                stream >> n;
                rect.setBottom(n);
                component._rect = rect;
            }
        }
    } while (!stream.atEnd());

    component._valid = false;
    return stream;
}

/**
 * @brief Operator to deserialise the component in Kicad format
 * @param stream input stream
 * @param component component to deserialise
 * @return stream
 */
QTextStream &operator<<(QTextStream &stream, const Component &component)
{
    // http://en.wikibooks.org/wiki/Kicad/file_formats#Description_of_a_component_2

    // comments
    stream << "#" << '\n' << "# " << component._name << '\n' << "#" << '\n';

    // def
    stream << "DEF " << component._name << " " << component._prefix << " 0 40 Y Y 1 F N" << '\n';
    stream << "F0 \"" << component._prefix << "\" " << component._rect.right()-50 << " " << -component._rect.bottom()-50 << " 50 H V C CNN" << '\n';
    stream << "F1 \"" << component._name << "\" 0 0 50 H V C CNN" << '\n';
    stream << "F2 \"~\" 0 0 50 H I C CNN" << '\n';
    stream << "F3 \"~\" 0 0 50 H I C CNN" << '\n';

    // footprints
    if (!component._footPrints.isEmpty())
    {
        stream << "$FPLIST" << '\n';
        foreach (QString footPrint, component._footPrints)
        {
            stream << " " << footPrint << '\n';
        }
        stream << "$ENDFPLIST" << '\n';
    }

    // alias
    if (!component._alias.isEmpty())
        stream << "ALIAS " << component._alias.join(" ") << '\n';

    stream << "DRAW" << '\n';
    // pins
    foreach (Pin *pin, component._pins)
    {
        stream << *pin << '\n';
    }

    // rect
    if (component._rect.isValid())
    {
        stream << "S "
               << component._rect.left() << " "
               << component._rect.top() << " "
               << component._rect.right() << " "
               << component._rect.bottom() << " "
               << "0 1 10 f" << '\n';
    }

    // end
    stream << "ENDDRAW" << '\n';
    stream << "ENDDEF";

    return stream;
}
