#include "datasheet.h"

#include <QDebug>
#include <QPainter>
#include <QFile>
#include <QDomNamedNodeMap>
#include <QDir>
#include "kicadexport.h"

#include <poppler/qt5/poppler-qt5.h>
#include <poppler/qt5/poppler-form.h>

using namespace Poppler;

Datasheet::Datasheet()
{
    _doc = NULL;
}

Datasheet::~Datasheet()
{
    close();
}

bool Datasheet::open(int dsid)
{
    return open(QString("edc/doc/%1g.pdf").arg(dsid));
}

bool Datasheet::open(QString fileName)
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

    return _doc != NULL;
}

void Datasheet::close()
{
    delete _doc;
    foreach (DatasheetPackage *package, _packages)
    {
        delete package;
    }
    _packages.clear();
}

void Datasheet::pinSearch(int numPage)
{
    QList<DatasheetBox *> numbers;
    QList<DatasheetBox *> labels;
    QList<DatasheetBox *> proc_labels;
    QList<DatasheetBox *> pack_labels;
    QList<DatasheetPin *> pins;
    QList<DatasheetPackage *> packages;

    qDebug() << "+page: " << numPage;
    if (_doc == NULL)
    {
        qDebug() << "can not open";
        return;
    }
    if (_doc->numPages() < 1)
    {
        qDebug() << "no page";
        return;
    }

    Poppler::Page *page = _doc->page(numPage);

    bool prev = false;
    DatasheetBox *box = new DatasheetBox();
    foreach (TextBox *textBox, page->textList())
    {
        bool okNumber;

        if (textBox->text().startsWith("•"))
            textBox->text().mid(1).toInt(&okNumber);
        else
            textBox->text().toInt(&okNumber);
        if (textBox->nextWord() != NULL && okNumber == false)
        {
            box->text.append(textBox->text());
            if (textBox->hasSpaceAfter())
                box->text.append(" ");
            box->pos = textBox->boundingBox().united(box->pos);
            prev = true;
        }
        else
        {
            if (prev && !okNumber)
            {
                box->text = box->text + textBox->text();
                box->pos = textBox->boundingBox().united(box->pos);
            }
            else
            {
                if (!prev)
                {
                    if (textBox->text().startsWith("•"))
                        box->text = textBox->text().mid(1);
                    else
                        box->text = textBox->text();
                    box->pos = textBox->boundingBox();
                }

                if (okNumber)
                {
                    if (!prev)
                    {
                        numbers.push_back(box);
                        box = new DatasheetBox();
                    }
                    else
                    {
                        DatasheetBox *nbox = new DatasheetBox();
                        if (textBox->text().startsWith("•"))
                            nbox->text = textBox->text().mid(1);
                        else
                            nbox->text = textBox->text();
                        nbox->pos = textBox->boundingBox();
                        numbers.push_back(nbox);
                        okNumber = false;
                    }
                }
                prev = false;
            }

            box->text.replace(QRegExp("\\([0-9]+\\)"), "");
            if (!okNumber || prev)
            {
                if (box->text.isEmpty() ||
                    box->text.contains("Note", Qt::CaseInsensitive) ||
                    box->text.contains(QRegExp("^\\([0-9]+\\)$")))
                {
                    // none
                }
                else if (box->text.startsWith("PIC", Qt::CaseInsensitive) ||
                         box->text.startsWith("DSPIC", Qt::CaseInsensitive) ||
                         box->text.startsWith("IS", Qt::CaseInsensitive) ||
                         box->text.startsWith("RX", Qt::CaseInsensitive))
                {
                    proc_labels.push_back(box);
                    box = new DatasheetBox();
                }
                else if (box->text.contains("DIP", Qt::CaseInsensitive) ||
                         box->text.contains("SOIC", Qt::CaseInsensitive) ||
                         box->text.contains("BGA", Qt::CaseInsensitive) ||
                         box->text.contains("TQFP", Qt::CaseInsensitive) ||
                         box->text.contains("LQP", Qt::CaseInsensitive) ||
                         box->text.contains("LQFP", Qt::CaseInsensitive) ||
                         box->text.contains("LGA", Qt::CaseInsensitive) ||
                         box->text.contains("QFN", Qt::CaseInsensitive))
                {
                    pack_labels.push_back(box);
                    box = new DatasheetBox();
                }
                else if (box->text.size() > 10 && !box->text.contains("/"))
                {
                    delete box;
                    box = new DatasheetBox();
                }
                else
                {
                    labels.push_back(box);
                    box = new DatasheetBox();
                }
            }
            prev = false;
        }
    }

    // pairing label and number to pin
    foreach (DatasheetBox *number, numbers)
    {
        DatasheetPin *pin = new DatasheetPin();
        qreal dist = 999999999999;
        QPointF center = number->pos.center();
        DatasheetBox *assocLabel = NULL;
        for (int i = 0; i < labels.count(); i++)
        {
            DatasheetBox *label = labels[i];
            if (label->associated)
                continue;

            if (!DatasheetBox::isAlign(*label, *number))
                continue;

            qreal newdist = label->distanceToPoint(center);
            if (newdist < dist)
            {
                dist = newdist;
                assocLabel = label;
            }
        }

        if (assocLabel)
        {
            assocLabel->associated = true;

            pin->pin = number->text.toInt();
            pin->numPos = number->pos;
            pin->numberBox = number;

            pin->name = assocLabel->text;
            pin->name.remove(QRegExp("\\([0-9]+\\)"));
            pin->name.remove(QRegExp(" +"));
            pin->nameBox = assocLabel;

            pin->pos = number->pos.united(assocLabel->pos);

            pins.push_back(pin);
        }
    }

    // painring pin to find package
    foreach (DatasheetPin *pin, pins)
    {
        if (pin->pin == 1)
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
                DatasheetPackage *nearPackage = NULL;
                qreal dist = 99999999999999;
                QPointF center = pin->numPos.center();
                foreach (DatasheetPackage *package, packages)
                {
                    DatasheetPin *lastpin = package->pins.last();
                    if (lastpin->pin == pin->pin || lastpin->pin + 4 < pin->pin)
                        continue;

                    qreal newdist = lastpin->distanceToPoint(center);
                    if (newdist < dist)
                    {
                        dist = newdist;
                        nearPackage = package;
                    }
                }
                if (nearPackage != NULL)
                {
                    nearPackage->pins.push_back(pin);
                }
            }
        }
    }

    // unasociated label
    foreach (DatasheetBox *label, labels)
    {
        if (label->associated == false)
        {
            DatasheetPin *nearPin = NULL;
            qreal dist = 99999999999999;
            //qDebug()<<"+ unasociated label"<<label->text;
            for (int i = 0; i < pins.count(); i++)
            {
                DatasheetPin *pin = pins[i];
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
            if (nearPin != NULL && dist < nearPin->numPos.height()*2)
            {
                //qDebug()<<"  > pin"<<nearPin->name<<nearPin->numberBox->text<<dist;
                nearPin->name += label->text;
                label->associated = true;
            }
        }
    }

    // ajust size of package
    foreach (DatasheetPackage *package, packages)
    {
        QRectF rect;
        QRectF rectNum;
        foreach (DatasheetPin *pin, package->pins)
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
    //_package.clear();
    foreach (DatasheetPackage *package, packages)
    {
        if (package->pins.count() < 5)
        {
            delete package;
            continue;
        }
        QRect rect = package->rect.toRect().adjusted(-50, -50, 50, 50);
        rect.adjust(-dec, -dec, dec, dec);
        package->image = _doc->page(numPage)->renderToImage(
            72.0 * res, 72.0 * res, rect.x() * res, rect.y() * res,
            rect.width() * res, rect.height() * res);
        QPainter painter(&package->image);
        qDebug() << "==============================";
        pac++;
        QDir dir;
        dir.mkdir(_name);
        QFile file(_name+QString("/p%1_pack%2.txt").arg(numPage + 1).arg(pac));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        qDebug() << _name+QString("/p%1_pack%2.txt").arg(numPage + 1).arg(pac);
        QTextStream textStream(&file);

        textStream << "Proc: ";
        foreach (DatasheetBox *proc, proc_labels)
        {
            textStream << proc->text << "-";
        }
        textStream << endl << "Package: ";
        foreach (DatasheetBox *pack, pack_labels)
        {
            textStream << pack->text << "-";
        }
        textStream << endl;

        painter.setPen(QPen(Qt::yellow, 2, Qt::DotLine));
        foreach (DatasheetBox *number, numbers)
        {
            painter.drawRect(
                QRect((number->pos.topLeft() - rect.topLeft()).toPoint() * res,
                      number->pos.size().toSize() * res));
        }
        foreach (DatasheetBox *label, labels)
        {
            if (label->associated == false)
                painter.setPen(QPen(Qt::green, 2, Qt::DotLine));
            else
                painter.setPen(QPen(Qt::blue, 2, Qt::DotLine));
            painter.drawRect(
                QRect((label->pos.topLeft() - rect.topLeft()).toPoint() * res,
                      label->pos.size().toSize() * res).adjusted(-2,-2,2,2));

        }
        painter.setPen(QPen(Qt::red, 2, Qt::DotLine));
        foreach (DatasheetPin *pin, package->pins)
        {
            textStream << pin->pin << "\t" << pin->name << endl;
            painter.drawRect(
                QRect((pin->pos.topLeft() - rect.topLeft()).toPoint() * res,
                      pin->pos.size().toSize() * res));
        }
        package->image.save(
            _name+QString("/p%1_pack%2.png").arg(numPage + 1).arg(pac));
        file.close();
        package->name =
            QString("Package p.%1 pack->%2").arg(numPage + 1).arg(pac);
        _packages.push_back(package);
    }
}

QString Datasheet::name() const
{
    return _name;
}

int Datasheet::pagePinDiagram(int pageStart, bool *bgaStyle)
{
    QStringList keyWords;
    keyWords << "Pin Diagram"
             << "PIN DESCRIPTION"
             << "PIN NAMES"
             << "Pin Assignment"
             << "PIN CONFIGURATION"
             << "PACKAGE"
             << "Interface pin description";

    if (_doc == NULL)
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
        return -1;
    for (int i = pageStart; i < _doc->numPages(); i++)
    {
        QList<QRectF> result;
        Poppler::Page *page = _doc->page(i);
        if (page == NULL)
            continue;

        bool vssOk = false;
        bool vddOk = false;
        bool labelOk = false;
        foreach (TextBox *textBox, page->textList())
        {
            QString text = textBox->text();
            if (textBox->nextWord())
            {
                if (textBox->hasSpaceAfter())
                    text.append(" ");
                text.append(textBox->nextWord()->text());
            }

            if (text.contains("vss", Qt::CaseInsensitive))
                vssOk = true;
            if (text.contains("vdd", Qt::CaseInsensitive))
                vddOk = true;
            /* if(text.contains("bga", Qt::CaseInsensitive))
                *bgaStyle=true;*/
            *bgaStyle = false;

            foreach (QString keyWord, keyWords)
            {
                if (text.contains(keyWord, Qt::CaseInsensitive))
                    labelOk = true;
            }

            if (labelOk && vssOk && vddOk)
                return i;
        }
    }
    return -1;
}

void Datasheet::analyse(int pageBegin, int pageEnd)
{
    int page = pageBegin;
    bool bgaStyle;

    if (pageBegin != -1 && pageEnd == -1) // search in one page
    {
        pinSearch(pageBegin-1);
        return;
    }

    while ((page = pagePinDiagram(page + 1, &bgaStyle)) != -1)
    {
        if (pageEnd != -1 && page >= pageEnd)
            return;
        /*if(bgaStyle)
            pinSearchBGA(page);
        else */
            pinSearch(page);
    }
}

const QList<DatasheetPackage *> &Datasheet::packages() const
{
    return _packages;
}
