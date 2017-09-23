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
    QList<DatasheetBox> numbers;
    QList<DatasheetBox> labels;
    QList<DatasheetBox> proc_labels;
    QList<DatasheetBox> pack_labels;
    QList<DatasheetPin> pins;
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
    // QRectF posPinDiagram=page->search("Pin Diagrams",
    // Poppler::Page::IgnoreCase).first();

    bool prev = false;
    DatasheetBox box;
    foreach (TextBox *textBox, page->textList())
    {
        bool okNumber;
        // if(textBox->boundingBox().top()<posPinDiagram.top()) continue;
        if (textBox->text().startsWith("•"))
            textBox->text().mid(1).toInt(&okNumber);
        else
            textBox->text().toInt(&okNumber);
        if (textBox->nextWord() != NULL && okNumber == false)
        {
            box.text.append(textBox->text());
            if (textBox->hasSpaceAfter())
                box.text.append(" ");
            box.pos = textBox->boundingBox().united(box.pos);
            prev = true;
        }
        else
        {
            if (prev)
            {
                box.text = box.text + textBox->text();
                box.pos = textBox->boundingBox().united(box.pos);
            }
            else
            {
                if (textBox->text().startsWith("•"))
                    box.text = textBox->text().mid(1);
                else
                    box.text = textBox->text();
                box.pos = textBox->boundingBox();
                prev = false;
                if (okNumber)
                {
                    numbers.push_back(box);
                }
            }
            box.text.replace(QRegExp("\\([0-9]+\\)"), "");

            if (!okNumber || prev)
            {
                if (box.text.isEmpty() ||
                    box.text.contains("Note", Qt::CaseInsensitive) ||
                    box.text.contains(QRegExp("^\\([0-9]+\\)$")))
                {
                    // none
                }
                else if (box.text.startsWith("PIC", Qt::CaseInsensitive) ||
                         box.text.startsWith("DSPIC", Qt::CaseInsensitive) ||
                         box.text.startsWith("IS", Qt::CaseInsensitive) ||
                         box.text.startsWith("RX", Qt::CaseInsensitive))
                {
                    proc_labels.push_back(box);
                    qDebug() << "proc_labels" << box.text;
                }
                else if (box.text.contains("DIP", Qt::CaseInsensitive) ||
                         box.text.contains("SOIC", Qt::CaseInsensitive) ||
                         box.text.contains("BGA", Qt::CaseInsensitive) ||
                         box.text.contains("TQFP", Qt::CaseInsensitive) ||
                         box.text.contains("LQP", Qt::CaseInsensitive) ||
                         box.text.contains("LQFP", Qt::CaseInsensitive) ||
                         box.text.contains("LGA", Qt::CaseInsensitive) ||
                         box.text.contains("QFN", Qt::CaseInsensitive))
                {
                    pack_labels.push_back(box);
                    qDebug() << "pack_labels" << box.text;
                }
                else if (box.text.size() > 10 && !box.text.contains("/"))
                {
                    // none
                }
                else
                {
                    labels.push_back(box);
                }
            }
            prev = false;
            box.text = "";
            box.pos = QRectF();
        }
    }

    // pairing label and number to pin
    foreach (DatasheetBox number, numbers)
    {
        DatasheetPin pin;
        qreal dist = 999999999999;
        QPointF center = number.pos.center();
        DatasheetBox assoc;
        int near = 0;
        for (int i = 0; i < labels.count(); i++)
        {
            DatasheetBox label = labels.at(i);
            if (!isAlign(label, number))
                continue;
            qreal newdist = (center - label.pos.center()).manhattanLength();
            if (newdist < dist)
            {
                dist = newdist;
                near = i;
            }
            newdist = (center - label.pos.bottomLeft()).manhattanLength();
            if (newdist < dist)
            {
                dist = newdist;
                near = i;
            }
            newdist = (center - label.pos.topRight()).manhattanLength();
            if (newdist < dist)
            {
                dist = newdist;
                near = i;
            }
            newdist = (center - label.pos.bottomRight()).manhattanLength();
            if (newdist < dist)
            {
                dist = newdist;
                near = i;
            }
            newdist = (center - label.pos.topLeft()).manhattanLength();
            if (newdist < dist)
            {
                dist = newdist;
                near = i;
            }
        }
        assoc = labels.at(near);
        labels.removeAt(near);

        pin.name = assoc.text.remove(QRegExp("\\([0-9]+\\)"));
        pin.pin = number.text.toInt();
        pin.pos = number.pos.united(assoc.pos);
        pin.numPos = number.pos;
        pins.push_back(pin);
    }

    // painring pin to find package
    foreach (DatasheetPin pin, pins)
    {
        if (pin.pin == 1)
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
            DatasheetPin pin = pins.at(i);
            if (pin.pin == pinNumber)
            {
                DatasheetPackage *nearPackage = NULL;
                qreal dist = 99999999999999;
                QPointF center = pin.numPos.center();
                foreach (DatasheetPackage *package, packages)
                {
                    DatasheetPin lastpin = package->pins.last();
                    // qDebug()<<dist<<pin.name<<pin.pin<<pin.pos<<lastpin.name;
                    if (lastpin.pin == pin.pin || lastpin.pin + 4 < pin.pin)
                        continue;
                    qreal newdist = (center - (lastpin.numPos.bottomLeft()))
                                        .manhattanLength();
                    if (newdist < dist)
                    {
                        dist = newdist;
                        nearPackage = package;
                    }
                    newdist =
                        (center - lastpin.numPos.topRight()).manhattanLength();
                    if (newdist < dist)
                    {
                        dist = newdist;
                        nearPackage = package;
                    }
                    newdist = (center - lastpin.numPos.bottomRight())
                                  .manhattanLength();
                    if (newdist < dist)
                    {
                        dist = newdist;
                        nearPackage = package;
                    }
                    newdist =
                        (center - lastpin.numPos.topLeft()).manhattanLength();
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

    // ajust size of package
    foreach (DatasheetPackage *package, packages)
    {
        QRectF rect;
        QRectF rectNum;
        foreach (DatasheetPin pin, package->pins)
        {
            rect = rect.united(pin.pos);
            rectNum = rectNum.united(pin.numPos);
        }
        package->rect = rect;
        package->rectNum = rectNum;
    }

    int pac = 0;
    int res = 8;
    int dec = 5;
    //_package.clear();
    foreach (DatasheetPackage *package, packages)
    {
        if (package->pins.count() < 5)
        {
            delete package;
            continue;
        }
        QRect rect = package->rect.toRect();
        rect.adjust(-dec, -dec, dec, dec);
        package->image = _doc->page(numPage)->renderToImage(
            72.0 * res, 72.0 * res, rect.x() * res, rect.y() * res,
            rect.width() * res, rect.height() * res);
        QPainter painter(&package->image);
        qDebug() << "==============================";
        pac++;
        QDir dir;
        dir.mkdir("img");
        QFile file(QString("img/p%1_pack%2.txt").arg(numPage + 1).arg(pac));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        qDebug() << QString("img/p%1_pack%2.txt").arg(numPage + 1).arg(pac);
        QTextStream textStream(&file);

        textStream << "Proc: ";
        foreach (DatasheetBox proc, proc_labels)
        {
            textStream << proc.text << "-";
        }
        textStream << endl << "Package: ";
        foreach (DatasheetBox pack, pack_labels)
        {
            textStream << pack.text << "-";
        }
        textStream << endl;

        painter.setPen(QPen(Qt::yellow, 5, Qt::DotLine));
        foreach (DatasheetBox number, numbers)
        {
            painter.drawRect(
                QRect((number.pos.topLeft() - rect.topLeft()).toPoint() * res,
                      number.pos.size().toSize() * res));
        }
        painter.setPen(QPen(Qt::blue, 5, Qt::DotLine));
        foreach (DatasheetBox label, labels)
        {
            painter.drawRect(
                QRect((label.pos.topLeft() - rect.topLeft()).toPoint() * res,
                      label.pos.size().toSize() * res));
        }
        painter.setPen(QPen(Qt::red, 5, Qt::DotLine));
        foreach (DatasheetPin pin, package->pins)
        {
            textStream << pin.pin << "\t" << pin.name << endl;
            painter.drawRect(
                QRect((pin.pos.topLeft() - rect.topLeft()).toPoint() * res,
                      pin.pos.size().toSize() * res));
        }
        package->image.save(
            QString("img/p%1_pack%2.png").arg(numPage + 1).arg(pac));
        file.close();
        package->name =
            QString("Package p.%1 pack.%2").arg(numPage + 1).arg(pac);
        _packages.push_back(package);
        KicadExport kicad;
        kicad.exportPack(
            package, QString("img/p%1_pack%2.lib").arg(numPage + 1).arg(pac));
        // qDebug()<<_packages.count();
    }
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
            // if(text.contains("bga", Qt::CaseInsensitive)) *bgaStyle=true;

            // if(i==6) qDebug()<<text<<labelOk<<vssOk<<vddOk;

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

void Datasheet::analyse()
{
    int page = -1;
    bool bgaStyle;
    while ((page = pagePinDiagram(page + 1, &bgaStyle)) != -1)
    {
        /*if(bgaStyle)
            pinSearchBGA(page);
        else */
            pinSearch(page);
        // return;
    }
}

bool Datasheet::isAlign(DatasheetBox &label, DatasheetBox &number)
{
    if (label.pos.width() > label.pos.height())  // Horizontal
    {
        qreal marge = label.pos.height();
        if (label.pos.top() - marge < number.pos.top() &&
            label.pos.bottom() + marge > number.pos.bottom())
            return true;
        else
            return false;
    }
    else  // Vertical
    {
        qreal marge = label.pos.width();
        if (label.pos.left() - marge < number.pos.left() &&
            label.pos.right() + marge > number.pos.right())
            return true;
        else
            return false;
    }
}

const QList<DatasheetPackage *> &Datasheet::packages() const
{
    return _packages;
}

DatasheetPackage::DatasheetPackage()
{
}

DatasheetPackage::~DatasheetPackage()
{
}

Component DatasheetPackage::toComponent() const
{
    Component comp;
    if (!icname.isEmpty())
        comp.setName(icname.first());
    for (int i = 1; i < icname.count(); i++) comp.addAlias(icname.at(i));

    foreach (DatasheetPin dpin, pins)
    {
        Pin pin;
        pin.setName(dpin.name);
        pin.setPadname(QString::number(dpin.pin));
        comp.addPin(pin);
    }

    return comp;
}
