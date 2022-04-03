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

#include "kicadlibparser.h"

#include <QDateTime>
#include <QDebug>
#include <QFileInfo>

#include "model/drawarc.h"
#include "model/drawcircle.h"
#include "model/drawpoly.h"
#include "model/drawrect.h"
#include "model/drawtext.h"

KicadLibParser::KicadLibParser()
{
}

Lib *KicadLibParser::loadLib(const QString &fileName, Lib *lib)
{
    bool mylib = false;
    if (lib == nullptr)
    {
        mylib = true;
        lib = new Lib();
    }

    QFile input(fileName);
    QFileInfo info(input);
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (mylib)
        {
            delete lib;
        }
        return Q_NULLPTR;
    }
    _stream.setDevice(&input);

    _stream.readLine();
    lib->clear();

    Component *component;
    do
    {
        component = readComponent();
        if (component != nullptr)
        {
            lib->addComponent(component);
        }
        else
        {
            _stream.readLine();
        }
    } while (!_stream.atEnd());
    lib->setName(info.baseName());

    _stream.setDevice(Q_NULLPTR);
    return lib;
}

bool KicadLibParser::saveLib(const QString &fileName, Lib *lib)
{
    QFile output(fileName);
    QFileInfo info(output);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    _stream.setDevice(&output);
    writeLib(lib);
    lib->setName(info.baseName());

    _stream.setDevice(Q_NULLPTR);
    return true;
}

QString KicadLibParser::readText()
{
    QString text;
    QString word;
    _stream >> word;
    if (word.startsWith('"'))
    {
        word.remove(0, 1);
        text.append(word);
        while (!word.endsWith('"') && _stream.status() == QTextStream::Ok)
        {
            _stream >> word;
            text.append(' ');
            text.append(word);
        }
        text.remove('"');
        return text;
    }

    return word;
}

/**
 * @brief Serialise the lib in Kicad format
 * @param lib lib to serialise
 */
void KicadLibParser::writeLib(Lib *lib)
{
    _stream << "EESchema-LIBRARY Version 2.3  Date: " << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") << '\n';
    _stream << "#encoding utf-8" << '\n';
    //_stream << "#created with uConfig by Sebastien CAUX (sebcaux)" << '\n';
    //_stream << "#https://github.com/Robotips/uConfig" << '\n';

    // components
    for (Component *component : lib->components())
    {
        writeComponent(component);
        _stream << '\n';
    }

    // footer
    _stream << "#" << '\n';
    _stream << "#End Library" << '\n';
}

void KicadLibParser::writeComponent(Component *component)
{
    // http://en.wikibooks.org/wiki/Kicad/file_formats#Description_of_a_component_2

    // comments
    _stream << "#" << '\n' << "# " << component->name() << '\n' << "#" << '\n';

    // def
    _stream << "DEF " << component->name() << " " << component->prefix() << " 0 20 ";
    _stream << (component->showPadName() ? "Y " : "N ");
    _stream << (component->showPinName() ? "Y " : "N ");
    _stream << "1 F N" << '\n';

    // F0
    _stream << "F0";
    writeLabel(component->refText());
    _stream << '\n';

    // F1
    _stream << "F1";
    writeLabel(component->nameText());
    _stream << '\n';

    // F2
    _stream << "F2";
    writeLabel(component->packageText());
    _stream << '\n';

    // F3
    _stream << "F3";
    writeLabel(component->docText());
    _stream << '\n';

    // footprints
    if (!component->footPrints().isEmpty())
    {
        _stream << "$FPLIST" << '\n';
        for (const QString &footPrint : component->footPrints())
        {
            _stream << " " << footPrint << '\n';
        }
        _stream << "$ENDFPLIST" << '\n';
    }

    // alias
    if (!component->aliases().isEmpty())
    {
        _stream << "ALIAS " << component->aliases().join(" ") << '\n';
    }

    _stream << "DRAW" << '\n';
    // draw
    for (Draw *draw : component->draws())
    {
        writeDraw(draw);
        _stream << '\n';
    }
    // pins
    for (Pin *pin : component->pins())
    {
        writePin(pin);
        _stream << '\n';
    }

    // end
    _stream << "ENDDRAW" << '\n';
    _stream << "ENDDEF";
}

void KicadLibParser::writePin(Pin *pin)
{
    // http://en.wikibooks.org/wiki/Kicad/file_formats#X_record_.28Pin.29

    // X PIN_NAME PAD_NAME X_POS Y_POS LINE_WIDTH DIRECTION NAME_TEXT_SIZE
    // LABEL_TEXT_SIZE LAYER ?1? ELECTRICAL_TYPE
    _stream << "X ";
    if (pin->name().isEmpty())
    {
        _stream << "~";
    }
    else
    {
        _stream << pin->name();
    }
    _stream << " " << pin->padName() << " "                     // pad name
            << pin->pos().x() << " " << -pin->pos().y() << " "  // x y position
            << pin->length() << " "                             // lenght
            << pin->directionString() << " "                    // pin direction (up/down/left/right)
            << "50"
            << " "  // name text size
            << "50"
            << " "  // pad name text size
            << pin->layer() << " "
            << "1"
            << " " << pin->electricalTypeString();
    if (pin->pinType() != Pin::Normal)
    {
        _stream << " " << pin->pinTypeString();
    }
}

void KicadLibParser::writeDraw(Draw *draw)
{
    DrawRect *drawRect;
    DrawText *drawText;

    switch (draw->type())
    {
        case Draw::TypeDrawArc:
            break;
        case Draw::TypeDrawCircle:
            break;
        case Draw::TypeDrawPoly:
            break;
        case Draw::TypeDrawRect:
            // S startx starty endx endy unit convert thickness fill
            drawRect = dynamic_cast<DrawRect *>(draw);
            _stream << "S " << drawRect->pos().x() << " " << -drawRect->pos().y() << " " << drawRect->endPos().x() << " " << -drawRect->endPos().y() << " " << drawRect->unit()
                    << " " << drawRect->convert() << " " << drawRect->thickness() << " ";
            switch (drawRect->filled())
            {
                case Draw::DrawNotFilled:
                    _stream << "N";
                    break;
                case Draw::DrawFilledForeGround:
                    _stream << "F";
                    break;
                case Draw::DrawFilledBackGround:
                    _stream << "f";
                    break;
            }
            break;

        case Draw::TypeDrawText:
            // T direction posx posy text_size text_type unit convert text text_italic text_bold text_hjustify text_vjustify
            drawText = dynamic_cast<DrawText *>(draw);
            _stream << "T ";
            if (drawText->direction() == DrawText::DirectionHorizontal)
            {
                _stream << "0 ";
            }
            else
            {
                _stream << "900 ";
            }

            _stream << drawText->pos().x() << " " << -drawText->pos().y() << " " << drawText->textSize() << " "
                    << "0 " << drawText->unit() << " " << drawText->convert() << " "
                    << "\"" << drawText->text() << "\""
                    << " ";

            if (drawText->textStyle().testFlag(DrawText::TextItalic))
            {
                _stream << "Italic ";
            }
            else
            {
                _stream << "Normal ";
            }

            if (drawText->textStyle().testFlag(DrawText::TextBold))
            {
                _stream << "1 ";
            }
            else
            {
                _stream << "0 ";
            }

            switch (drawText->textHJustify())
            {
                case DrawText::TextHCenter:
                    _stream << "C ";
                    break;
                case DrawText::TextHLeft:
                    _stream << "L ";
                    break;
                case DrawText::TextHRight:
                    _stream << "R ";
                    break;
            }

            switch (drawText->textVJustify())
            {
                case DrawText::TextVCenter:
                    _stream << "C";
                    break;
                case DrawText::TextVBottom:
                    _stream << "B";
                    break;
                case DrawText::TextVTop:
                    _stream << "T";
                    break;
            }

            break;
        default:
            break;
    }
}

void KicadLibParser::writeLabel(DrawText *draw)
{
    _stream << " \"" << draw->text() << "\" ";

    _stream << draw->pos().x() << " " << -draw->pos().y() << " " << draw->textSize() << " ";

    if (draw->direction() == DrawText::DirectionHorizontal)
    {
        _stream << "H ";
    }
    else
    {
        _stream << "V ";
    }

    if (draw->isVisible())
    {
        _stream << "V ";
    }
    else
    {
        _stream << "I ";
    }

    switch (draw->textHJustify())
    {
        case DrawText::TextHCenter:
            _stream << "C ";
            break;
        case DrawText::TextHLeft:
            _stream << "L ";
            break;
        case DrawText::TextHRight:
            _stream << "R ";
            break;
    }

    switch (draw->textVJustify())
    {
        case DrawText::TextVCenter:
            _stream << "C";
            break;
        case DrawText::TextVBottom:
            _stream << "B";
            break;
        case DrawText::TextVTop:
            _stream << "T";
            break;
    }

    if (draw->textStyle().testFlag(DrawText::TextItalic))
    {
        _stream << "I";
    }
    else
    {
        _stream << "N";
    }

    if (draw->textStyle().testFlag(DrawText::TextBold))
    {
        _stream << "B";
    }
    else
    {
        _stream << "N";
    }
}

Component *KicadLibParser::readComponent()
{
    Component *component = new Component();

    QString dummy;
    bool draw = false;
    do
    {
        QString start;
        _stream >> start;
        if (start.at(0) == '#')  // comment
        {
            _stream.readLine();
        }
        else if (start == "DEF")
        {
            QString name;
            _stream >> name;
            component->setName(name);

            QString prefix;
            _stream >> prefix;
            component->setPrefix(prefix);

            _stream >> dummy;
            _stream >> dummy;  // text offset TODO

            QString option;
            _stream >> option;
            component->setShowPadName(option == "Y");
            _stream >> option;
            component->setShowPinName(option == "Y");

            int unitsCount = 1;
            _stream >> unitsCount;
            component->setUnitCount(unitsCount);

            _stream.readLine();
        }
        else if (start.startsWith("F"))
        {
            if (start.startsWith("F0"))
            {
                DrawText *refText = readLabel();
                if (refText != nullptr)
                {
                    component->setRefText(refText);
                }
                else
                {
                    _stream.readLine();
                }
            }
            else if (start.startsWith("F1"))
            {
                DrawText *nameText = readLabel();
                if (nameText != nullptr)
                {
                    component->setNameText(nameText);
                }
                else
                {
                    _stream.readLine();
                }
            }
            else if (start.startsWith("F2"))
            {
                DrawText *packageText = readLabel();
                if (packageText != nullptr)
                {
                    component->setPackageText(packageText);
                }
                else
                {
                    _stream.readLine();
                }
            }
            else if (start.startsWith("F3"))
            {
                DrawText *docText = readLabel();
                if (docText != nullptr)
                {
                    component->setDocText(docText);
                }
                else
                {
                    _stream.readLine();
                }
            }
            else
            {
                _stream.readLine();
            }
        }
        else if (start == "$FPLIST")
        {
            QString footprint;
            while (!_stream.atEnd())
            {
                _stream >> footprint;
                if (footprint == "$ENDFPLIST")
                {
                    break;
                }
                component->addFootPrint(footprint);
            }
        }
        else if (start.startsWith("DRAW"))
        {
            draw = true;
            _stream.readLine();
        }
        else if (start.startsWith("ALIAS"))
        {
            QString aliases = _stream.readLine();
            component->addAlias(aliases.split(' ', QString::SkipEmptyParts));
        }
        else if (start.startsWith("ENDDRAW"))
        {
            draw = false;
            _stream.readLine();
        }
        else if (start.startsWith("ENDDEF"))
        {
            draw = false;
            _stream.readLine();
            return component;
        }
        else if (draw)
        {
            if (start.startsWith('X'))
            {
                Pin *pin = readPin();
                if (pin != nullptr)
                {
                    component->addPin(pin);
                }
                else
                {
                    _stream.readLine();
                }
            }
            else
            {
                Draw *draw = readDraw(start.data()[0].toLatin1());
                if (draw != nullptr)
                {
                    component->addDraw(draw);
                }
                else
                {
                    _stream.readLine();
                }
            }
        }
    } while (!_stream.atEnd());

    return Q_NULLPTR;
}

Pin *KicadLibParser::readPin()
{
    Pin *pin = new Pin();
    _stream.resetStatus();

    // name
    QString name;
    _stream >> name;
    if (name == "~")
    {
        name = "";
    }
    if (_stream.status() != QTextStream::Ok)
    {
        delete pin;
        return Q_NULLPTR;
    }
    pin->setName(name);

    // name
    QString padName;
    _stream >> padName;
    if (_stream.status() != QTextStream::Ok)
    {
        delete pin;
        return Q_NULLPTR;
    }
    pin->setPadName(padName);

    // position
    int x;
    int y;
    _stream >> x >> y;
    if (_stream.status() != QTextStream::Ok)
    {
        delete pin;
        return Q_NULLPTR;
    }
    pin->setPos(x, -y);

    // lenght
    int lenght;
    _stream >> lenght;
    if (_stream.status() != QTextStream::Ok)
    {
        delete pin;
        return Q_NULLPTR;
    }
    pin->setLength(lenght);

    // orientation
    char direction;
    _stream.skipWhiteSpace();
    _stream >> direction;
    pin->setDirection(direction);

    // text size
    int textNameSize;
    int textPadSize;
    _stream >> textPadSize;
    _stream >> textNameSize;
    pin->setTextNameSize(textNameSize);
    pin->setTextPadSize(textPadSize);

    // layer
    int layer;
    _stream >> layer;
    if (_stream.status() != QTextStream::Ok)
    {
        delete pin;
        return Q_NULLPTR;
    }
    pin->setLayer(layer);

    QString dummy;
    _stream.skipWhiteSpace();
    _stream >> dummy;

    // elec type
    char elec_type;
    _stream.skipWhiteSpace();
    _stream >> elec_type;
    pin->setElectricalType(elec_type);

    // pin type
    QString pinType = _stream.readLine();
    pin->setPinType(pinType.trimmed());

    return pin;
}

Draw *KicadLibParser::readDraw(char c)
{
    int n;
    char nc;
    QString text;
    _stream.resetStatus();

    switch (c)
    {
        case 'S':  // rect
        {
            DrawRect *draw = new DrawRect();
            _stream >> n;
            draw->pos().setX(n);
            _stream >> n;
            draw->pos().setY(-n);
            _stream >> n;
            draw->endPos().setX(n);
            _stream >> n;
            draw->endPos().setY(-n);
            _stream >> n;
            draw->setUnit(n);
            _stream >> n;
            draw->setConvert(n);
            _stream >> n;
            draw->setThickness(n);

            _stream.skipWhiteSpace();
            _stream >> nc;
            switch (nc)
            {
                case 'F':
                    draw->setFilled(DrawText::DrawFilledForeGround);
                    break;
                case 'f':
                    draw->setFilled(DrawText::DrawFilledBackGround);
                    break;
                default:
                    draw->setFilled(DrawText::DrawNotFilled);
                    break;
            }
            _stream.readLine();
            return draw;
        }
        case 'T':  // text
        {
            DrawText *draw = new DrawText();
            _stream >> n;
            if (n == 0)
            {
                draw->setDirection(DrawText::DirectionHorizontal);
            }
            else
            {
                draw->setDirection(DrawText::DirectionVertital);
            }

            _stream >> n;
            draw->pos().setX(n);

            _stream >> n;
            draw->pos().setY(-n);

            _stream >> n;
            draw->setTextSize(n);

            _stream >> n;
            _stream >> n;
            draw->setUnit(n);

            _stream >> n;
            draw->setConvert(n);

            text = readText();
            draw->setText(text);

            DrawText::TextStyles style = DrawText::TextNormal;
            _stream >> text;
            if (text != "Normal" && text != "0")
            {
                style |= DrawText::TextItalic;
            }

            _stream >> n;
            if (n != 0)
            {
                style |= DrawText::TextBold;
            }
            draw->setTextStyle(style);

            _stream.skipWhiteSpace();
            _stream >> nc;
            switch (nc)
            {
                case 'C':
                    draw->setTextHJustify(DrawText::TextHCenter);
                    break;
                case 'R':
                    draw->setTextHJustify(DrawText::TextHRight);
                    break;
                default:
                    draw->setTextHJustify(DrawText::TextHLeft);
                    break;
            }

            _stream.skipWhiteSpace();
            _stream >> nc;
            switch (nc)
            {
                case 'B':
                    draw->setTextVJustify(DrawText::TextVBottom);
                    break;
                case 'T':
                    draw->setTextVJustify(DrawText::TextVTop);
                    break;
                default:
                    draw->setTextVJustify(DrawText::TextVCenter);
                    break;
            }
            _stream.readLine();
            return draw;
        }
        case 'P':  // Poly
        {
            DrawPoly *draw = new DrawPoly();
            int ptCount = 0;
            _stream >> ptCount;
            _stream >> n;
            draw->setUnit(n);
            _stream >> n;
            draw->setConvert(n);
            _stream >> n;
            draw->setThickness(n);

            for (int i = 0; i < ptCount; i++)
            {
                QPoint pt;
                _stream >> n;
                pt.setX(n);
                _stream >> n;
                pt.setY(-n);
                draw->points().append(pt);
            }

            _stream.skipWhiteSpace();
            _stream >> nc;
            switch (nc)
            {
                case 'F':
                    draw->setFilled(DrawText::DrawFilledForeGround);
                    break;
                case 'f':
                    draw->setFilled(DrawText::DrawFilledBackGround);
                    break;
                default:
                    draw->setFilled(DrawText::DrawNotFilled);
                    break;
            }
            _stream.readLine();
            return draw;
        }
        case 'C':  // circle
        {
            DrawCircle *draw = new DrawCircle();
            _stream >> n;
            draw->pos().setX(n);
            _stream >> n;
            draw->pos().setY(-n);
            _stream >> n;
            draw->setRadius(n);
            _stream >> n;
            draw->setUnit(n);
            _stream >> n;
            draw->setConvert(n);
            _stream >> n;
            draw->setThickness(n);

            _stream.skipWhiteSpace();
            _stream >> nc;
            switch (nc)
            {
                case 'F':
                    draw->setFilled(Draw::DrawFilledForeGround);
                    break;
                case 'f':
                    draw->setFilled(Draw::DrawFilledBackGround);
                    break;
                default:
                    draw->setFilled(Draw::DrawNotFilled);
                    break;
            }
            _stream.readLine();
            return draw;
        }
        default:
            return Q_NULLPTR;
    }
}

DrawText *KicadLibParser::readLabel()
{
    int n;
    char nc;
    QString text;

    _stream.resetStatus();

    DrawText *draw = new DrawText();
    text = readText();
    draw->setText(text);
    _stream >> n;
    draw->pos().setX(n);
    _stream >> n;
    draw->pos().setY(-n);

    _stream >> n;
    draw->setTextSize(n);
    _stream.skipWhiteSpace();
    _stream >> nc;
    if (nc == 'H')
    {
        draw->setDirection(DrawText::DirectionHorizontal);
    }
    else
    {
        draw->setDirection(DrawText::DirectionVertital);
    }

    _stream.skipWhiteSpace();
    _stream >> nc;
    draw->setVisible(nc == 'V');

    _stream.skipWhiteSpace();
    _stream >> nc;
    switch (nc)
    {
        case 'C':
            draw->setTextHJustify(DrawText::TextHCenter);
            break;
        case 'R':
            draw->setTextHJustify(DrawText::TextHRight);
            break;
        default:
            draw->setTextHJustify(DrawText::TextHLeft);
            break;
    }

    _stream.skipWhiteSpace();
    _stream >> nc;
    switch (nc)
    {
        case 'B':
            draw->setTextVJustify(DrawText::TextVBottom);
            break;
        case 'T':
            draw->setTextVJustify(DrawText::TextVTop);
            break;
        default:
            draw->setTextVJustify(DrawText::TextVCenter);
            break;
    }

    _stream >> nc;
    DrawText::TextStyles style = DrawText::TextNormal;
    if (nc == 'I')
    {
        style |= DrawText::TextItalic;
    }
    _stream >> nc;
    if (nc == 'B')
    {
        style |= DrawText::TextBold;
    }
    draw->setTextStyle(style);

    _stream.readLine();
    return draw;
}
