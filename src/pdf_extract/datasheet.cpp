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

#include "datasheet.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QDomNamedNodeMap>
#include <QFile>
#include <QPainter>
#include <QRegularExpression>

#include <poppler/qt5/poppler-form.h>
#include <poppler/qt5/poppler-qt5.h>

using namespace Poppler;

Datasheet::Datasheet()
{
    _doc = nullptr;
    _debug = false;
    _force = false;
}

Datasheet::~Datasheet()
{
    close();
    clean();
}

bool Datasheet::open(const QString &fileName)
{
    close();
    _doc = Poppler::Document::load(fileName);
    _name = QFileInfo(fileName).baseName();

    /*QDomElement child=_doc->toc()->firstChild().toElement();
    while(!child.isNull())
    {
        qDebug()<<child.nodeName();
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
        {
            qDebug()<<'\t'<<grandChild.nodeName()<<grandChild.attribute("DestinationName");
            QDomElement subChild=grandChild.firstChild().toElement();
            while(!subChild.isNull())
            {
                qDebug()<<"\t\t"<<subChild.nodeName()<<subChild.attribute("DestinationName");
                subChild = subChild.nextSibling().toElement();
            }
            grandChild = grandChild.nextSibling().toElement();
        }
        child = child.nextSibling().toElement();
    }*/

    return _doc != nullptr;
}

void Datasheet::close()
{
    delete _doc;
    _doc = nullptr;
}

void Datasheet::pinSearch(int numPage)
{
    QList<DatasheetPin *> pins;
    QList<DatasheetPackage *> packages;

    emit log("==============================");

    /*for (DatasheetBox *box : _labels)
        if (!box->associated)
            delete box;*/
    _labels.clear();
    /*for (DatasheetBox *box : _numbers)
        if (!box->associated)
            delete box;*/
    _numbers.clear();
    /*for (DatasheetBox *box : _proc_labels)
        if (!box->associated)
            delete box;*/
    _proc_labels.clear();
    /*for (DatasheetBox *box : _pack_labels)
        if (!box->associated)
            delete box;*/
    _pack_labels.clear();

    pins = extractPins(numPage);
    // pins.append(extractPins(numPage + 1));
    QCoreApplication::processEvents();

    // painring pin to find package
    for (DatasheetPin *pin : pins)
    {
        if (pin->pin == 1 && pin->page == numPage)
        {
            DatasheetPackage *package;
            package = new DatasheetPackage();
            package->pins.push_back(pin);
            packages.push_back(package);
        }
    }
    for (int pinNumber = 2; pinNumber < 200; pinNumber++)
    {
        for (int i = 0; i < pins.count(); i++)
        {
            DatasheetPin *pin = pins.at(i);
            if (pin->pin == pinNumber)
            {
                DatasheetPackage *nearPackage = nullptr;
                qreal dist = 99999999999999;
                QPointF center = pin->numPos.center();
                for (DatasheetPackage *package : packages)
                {
                    DatasheetPin *lastpin = package->pins.last();
                    if (lastpin->pin == pin->pin || lastpin->pin + 4 < pin->pin)
                    {
                        continue;
                    }

                    qreal newdist = lastpin->distanceToPoint(center);
                    if (newdist < dist)
                    {
                        dist = newdist;
                        nearPackage = package;
                    }
                }
                if (nearPackage != nullptr)
                {
                    nearPackage->pins.push_back(pin);
                }
            }
        }
    }
    QCoreApplication::processEvents();

    // unasociated label
    for (DatasheetBox *label : _labels)
    {
        if (label->associated)
        {
            continue;
        }

        DatasheetPin *nearPin = nullptr;
        qreal dist = 99999999999999;
        // qDebug()<<"+ unasociated label"<<label->text;
        for (int i = 0; i < pins.count(); i++)
        {
            DatasheetPin *pin = pins[i];

            if (pin->page != label->page)
            {
                continue;
            }

            if (pin->name.endsWith("/"))
            {
                qreal newdist = label->distanceToPoint(pin->nameBox->pos.bottomLeft());
                if (newdist < dist)
                {
                    dist = newdist;
                    nearPin = pin;
                }
            }
        }
        if (nearPin != nullptr && dist < nearPin->numPos.height() * 2)
        {
            // qDebug()<<"  > pin"<<nearPin->name<<nearPin->numberBox->text<<dist;
            nearPin->name += label->text;
            label->associated = true;
        }
    }

    // ajust size of package
    for (DatasheetPackage *package : packages)
    {
        QRectF rect;
        QRectF rectNum;
        for (DatasheetPin *pin : package->pins)
        {
            rect = rect.united(pin->pos);
            rectNum = rectNum.united(pin->numPos);
        }
        package->rect = rect;
        package->rectNum = rectNum;
    }

    int pac = 0;
    int res = 4;
    int dec = 5;
    int count = 0;
    int badcount = 0;
    //_package.clear();
    for (DatasheetPackage *package : packages)
    {
        // package with less than 5 pin are deleted
        if (package->pins.count() < 5)
        {
            badcount++;
            delete package;
            continue;
        }
        // package pin 1 very far to pin 2 are deleted
        QPointF p1center = package->pins[1]->numberBox->pos.center();
        if (package->pins[0]->numberBox->distanceToPoint(p1center) > 50)
        {
            badcount++;
            delete package;
            continue;
        }
        count++;

        package->name = QString("p.%1_pack%2").arg(numPage + 1).arg(pac);
        _packages.push_back(package);
        pac++;

        if (!_debug)
        {
            continue;
        }

        QRect rect = package->rect.toRect().adjusted(-50, -50, 50, 50);
        rect.adjust(-dec, -dec, dec, dec);
        package->image = _doc->page(numPage)->renderToImage(72.0 * res, 72.0 * res, rect.x() * res, rect.y() * res, rect.width() * res, rect.height() * res);
        QPainter painter(&package->image);
        QDir dir;
        dir.mkdir(_name);

        painter.setPen(QPen(Qt::yellow, 2, Qt::DotLine));
        for (DatasheetBox *number : _numbers)
        {
            painter.drawRect(QRect((number->pos.topLeft() - rect.topLeft()).toPoint() * res, number->pos.size().toSize() * res));
        }
        for (DatasheetBox *label : _labels)
        {
            if (!label->associated)
            {
                painter.setPen(QPen(Qt::green, 2, Qt::DotLine));
            }
            else
            {
                painter.setPen(QPen(Qt::blue, 2, Qt::DotLine));
            }
            painter.drawRect(QRect((label->pos.topLeft() - rect.topLeft()).toPoint() * res, label->pos.size().toSize() * res).adjusted(-2, -2, 2, 2));
        }
        /*painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
            for (DatasheetPin *pin : package->pins)
            {
                painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
                painter.drawRect(QRect((pin->pos.topLeft() - rect.topLeft()).toPoint() * res,
                          pin->pos.size().toSize() * res));
                painter.setPen(QPen(Qt::yellow, 2, Qt::DotLine));
                painter.drawRect(QRect((pin->numPos.topLeft() - rect.topLeft()).toPoint() * res,
                                        pin->numPos.size().toSize() * res));
                painter.setPen(QPen(Qt::blue, 2, Qt::DotLine));
                painter.drawRect(QRect((pin->nameBox->pos.topLeft() - rect.topLeft()).toPoint() * res,
                                        pin->nameBox->pos.size().toSize() * res).adjusted(-2, -2, 2, 2));
            }*/
        package->image.save(_name + QString("/p%1_pack%2.png").arg(numPage + 1).arg(pac));
        QCoreApplication::processEvents();
    }

    emit log(QString("%1 package%2 deleted").arg(badcount).arg(badcount > 1 ? "s" : ""));
    emit log(QString("%1 package%2 found").arg(count).arg(count > 1 ? "s" : ""));
}

QRectF Datasheet::toGlobalPos(const QRectF &rect, Poppler::Page *page, int pageNumber)
{
    QRectF global = rect;
    global.setY(rect.y() + page->pageSize().height() * pageNumber);
    return rect;
}

QList<DatasheetPin *> Datasheet::extractPins(int numPage)
{
    QList<DatasheetPin *> pins;

    emit log(QString("+ find pins at page: %1").arg(numPage + 1));
    if (_doc == nullptr)
    {
        qDebug() << "can not open";
        return pins;
    }
    if (_doc->numPages() < 1 || numPage >= _doc->numPages())
    {
        qDebug() << "no page";
        return pins;
    }

    Poppler::Page *page = _doc->page(numPage);
    if (page == nullptr)
    {
        return pins;
    }

    // extracting text boxes and sort it by possible usage
    bool prev = false;
    DatasheetBox *box = new DatasheetBox();
    box->page = numPage;
    for (TextBox *textBox : page->textList())
    {
        bool okNumber;

        if (textBox->text().startsWith("•"))
        {
            textBox->text().mid(1).toInt(&okNumber);
        }
        else
        {
            textBox->text().toInt(&okNumber);
        }

        if (textBox->nextWord() != nullptr && !okNumber && DatasheetBox::isAlign(DatasheetBox(*textBox->nextWord()), DatasheetBox(*textBox)))
        {
            box->text.append(textBox->text());
            if (textBox->hasSpaceAfter())
            {
                box->text.append(" ");
            }
            box->pos = toGlobalPos(textBox->boundingBox(), page, numPage).united(box->pos);
            prev = true;
        }
        else
        {
            if (prev && !okNumber)
            {
                box->text = box->text + textBox->text();
                box->pos = toGlobalPos(textBox->boundingBox(), page, numPage).united(box->pos);
            }
            else
            {
                if (!prev)
                {
                    if (textBox->text().startsWith("•"))
                    {
                        box->text = textBox->text().mid(1);
                    }
                    else
                    {
                        box->text = textBox->text();
                    }
                    box->pos = toGlobalPos(textBox->boundingBox(), page, numPage);
                    prev = false;
                }

                if (okNumber)
                {
                    if (!prev)
                    {
                        _numbers.push_back(box);
                        box = new DatasheetBox();
                        box->page = numPage;
                    }
                    else
                    {
                        DatasheetBox *nbox = new DatasheetBox();
                        nbox->page = numPage;
                        if (textBox->text().startsWith("•"))
                        {
                            nbox->text = textBox->text().mid(1);
                        }
                        else
                        {
                            nbox->text = textBox->text();
                        }
                        nbox->pos = textBox->boundingBox();
                        _numbers.push_back(nbox);
                        okNumber = false;
                    }
                    prev = false;
                }
            }

            // remove notes
            box->text.replace(QRegularExpression("\\([0-9]+\\)"), "");

            // classify boxes
            if (!okNumber || prev)
            {
                if (box->text.isEmpty() || box->text.contains("Note", Qt::CaseInsensitive) || box->text.contains(QRegularExpression("^\\([0-9]+\\)$")))
                {
                    // none
                }
                /*else if (box->text.startsWith("PIC", Qt::CaseInsensitive) ||
                                 box->text.startsWith("DSPIC", Qt::CaseInsensitive) ||
                                 box->text.startsWith("IS", Qt::CaseInsensitive) ||
                                 box->text.startsWith("RX", Qt::CaseInsensitive))
                        {
                            _proc_labels.push_back(box);
                            box = new DatasheetBox();
                            box->page = numPage;
                        }*/
                else if (box->text.contains("DIP", Qt::CaseInsensitive) || box->text.contains("SOIC", Qt::CaseInsensitive)
                         || box->text.contains("BGA", Qt::CaseInsensitive) || box->text.contains("TQFP", Qt::CaseInsensitive)
                         || box->text.contains("LQP", Qt::CaseInsensitive) || box->text.contains("LQFP", Qt::CaseInsensitive)
                         || box->text.contains("LGA", Qt::CaseInsensitive) || box->text.contains("QFN", Qt::CaseInsensitive))
                {
                    _pack_labels.push_back(box);
                    box = new DatasheetBox();
                    box->page = numPage;
                }
                else if (box->text.size() > 10 && (!box->text.contains("/") && !box->text.contains("_") && !box->text.contains(",")))
                {
                    // qDebug()<<"filter long label"<<box->text;
                    delete box;
                    box = new DatasheetBox();
                    box->page = numPage;
                }
                else
                {
                    _labels.push_back(box);
                    box = new DatasheetBox();
                    box->page = numPage;
                }
            }
            prev = false;
        }
        delete textBox;
    }

    // pairing label and number to pin
    for (DatasheetBox *number : _numbers)
    {
        DatasheetPin *pin = new DatasheetPin();
        qreal dist = 999999999999;
        QPointF center = number->pos.center();
        DatasheetBox *assocLabel = nullptr;

        for (int i = 0; i < _labels.count(); i++)
        {
            DatasheetBox *label = _labels[i];

            if (label->associated)
            {
                continue;
            }

            if (label->page != number->page)
            {
                continue;
            }

            if (!DatasheetBox::isAlign(*label, *number))
            {
                continue;
            }

            qreal newdist = label->distanceToPoint(center);
            if (newdist < dist)
            {
                dist = newdist;
                assocLabel = label;
            }
        }

        if (assocLabel != nullptr)
        {
            assocLabel->associated = true;
            number->associated = true;

            pin->pin = number->text.toInt();
            pin->numPos = number->pos;
            pin->numberBox = number;

            pin->name = assocLabel->text;
            pin->name.remove(QRegularExpression("\\([0-9]+\\)"));
            pin->name.remove(QRegularExpression(" +"));
            pin->nameBox = assocLabel;

            pin->page = number->page;
            pin->pos = number->pos.united(assocLabel->pos);

            pins.push_back(pin);
        }
    }

    delete page;
    return pins;
}

bool Datasheet::debugEnabled() const
{
    return _debug;
}

void Datasheet::setDebugEnabled(bool debug)
{
    _debug = debug;
}

bool Datasheet::forceEnabled() const
{
    return _force;
}

void Datasheet::setForceEnabled(bool force)
{
    _force = force;
}

int Datasheet::pageCount() const
{
    if (_doc == nullptr)
    {
        return 0;
    }
    return _doc->numPages();
}

QImage Datasheet::pageThumbnail(int numPage) const
{
    QImage image;
    if (_doc == nullptr)
    {
        return QImage();
    }

    Poppler::Page *page = _doc->page(numPage);
    return page->renderToImage(20, 20, 0, 0, -1, -1);
}

void Datasheet::clean()
{
    /*for (DatasheetBox *box : _labels)
        if (!box->associated)
            delete box;*/
    _labels.clear();
    /*for (DatasheetBox *box : _numbers)
        if (!box->associated)
            delete box;*/
    _numbers.clear();
    /*for (DatasheetBox *box : _proc_labels)
        if (!box->associated)
            delete box;*/
    _proc_labels.clear();
    /*for (DatasheetBox *box : _pack_labels)
        if (!box->associated)
            delete box;*/
    _pack_labels.clear();
    for (DatasheetPackage *box : _packages)
    {
        delete box;
    }
    _packages.clear();
}

QString Datasheet::name() const
{
    return _name;
}

int Datasheet::pagePinDiagram(int pageStart, int pageEnd, bool *bgaStyle)
{
    QStringList keyWords;
    keyWords << "Pin Diagram"
             << "PIN DESCRIPTION"
             << "PIN NAMES"
             << "Pin Assignment"
             << "lead Assignment"
             << "PIN CONFIGURATION"
             << "Pinouts"
             << "PACKAGE"
             << "TQFP"
             << "Flatpack"
             << "MLF"
             << "DIP"
             << "PLCC"
             << "SOIC";

    if (_doc == nullptr)
    {
        qDebug() << "can not open";
        return -1;
    }
    if (_doc->numPages() < 1)
    {
        qDebug() << "no page";
        return -1;
    }

    if (pageStart >= _doc->numPages())
    {
        return -1;
    }
    for (int i = pageStart; i < _doc->numPages() && i <= pageEnd; i++)
    {
        Poppler::Page *page = _doc->page(i);
        if (page == nullptr)
        {
            continue;
        }

        emit pageChanged(i + 1);

        bool vssOk = false;
        bool vddOk = false;
        bool labelOk = false;
        for (TextBox *textBox : page->textList())
        {
            QString text = textBox->text();
            if (textBox->nextWord() != nullptr)
            {
                if (textBox->hasSpaceAfter())
                {
                    text.append(" ");
                }
                text.append(textBox->nextWord()->text());
            }

            if (text.contains("vss", Qt::CaseInsensitive) || text.contains("gnd", Qt::CaseInsensitive))
            {
                vssOk = true;
            }
            if (text.contains("vdd", Qt::CaseInsensitive) || text.contains("vcc", Qt::CaseInsensitive) || text.contains("vs", Qt::CaseInsensitive))
            {
                vddOk = true;
            }
            /* if (text.contains("bga", Qt::CaseInsensitive))
             *bgaStyle=true;*/
            *bgaStyle = false;

            for (const QString &keyWord : keyWords)
            {
                if (text.contains(keyWord, Qt::CaseInsensitive))
                {
                    labelOk = true;
                    break;
                }
            }
            delete textBox;

            if (labelOk && (vssOk || vddOk))
            {
                delete page;
                return i;
            }
        }
        delete page;
        QCoreApplication::processEvents();
    }

    return -1;
}

/**
 * @brief Launch analyse on datasheet.
 * * analyse() -> whole document analyse
 * * analyse(page) -> analyse only page `page`
 * * analyse(start, end) -> analyse from page `start` to page `end`
 * @param pageBegin
 * @param pageEnd
 */
void Datasheet::analyse(int pageBegin, int pageEnd)
{
    int page = pageBegin;
    bool bgaStyle;
    int mpageEnd = pageEnd;

    // only one page
    if (pageBegin != -1 && pageEnd == -1)  // search in one page
    {
        pinSearch(pageBegin);
    }
    else if (!_force)
    {
        if (mpageEnd == -1)
        {
            mpageEnd = pageCount();
        }
        if (page == -1)
        {
            page = 0;
        }
        while ((page = pagePinDiagram(page, mpageEnd, &bgaStyle)) != -1)
        {
            if (pageEnd != -1 && page >= pageEnd)
            {
                return;
            }
            pinSearch(page);
            page++;
        }
    }
    else
    {
        if (mpageEnd == -1)
        {
            mpageEnd = pageCount();
        }
        for (; page <= mpageEnd; page++)
        {
            pinSearch(page);
        }
    }
}

const QList<DatasheetPackage *> &Datasheet::packages() const
{
    return _packages;
}

QList<Component *> Datasheet::components()
{
    QList<Component *> components;
    for (DatasheetPackage *package : _packages)
    {
        Component *component = package->toComponent();
        component->reorganizeToPackageStyle();
        components.append(component);
        component->setDebugInfo(package->image);
    }
    return components;
}
