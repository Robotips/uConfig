#include "datasheet.h"

#include <QDebug>
#include <QPainter>
#include <QFile>
#include <QDomNamedNodeMap>
#include "kicadexport.h"

using namespace Poppler;

Datasheet::Datasheet()
{
    _doc=NULL;
}

Datasheet::~Datasheet()
{
    close();
}

void Datasheet::open(int dsid)
{
    open(QString("edc/doc/%1g.pdf").arg(dsid));
}

void Datasheet::open(QString fileName)
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

}

void Datasheet::close()
{
    delete _doc;
    foreach(Package *package, _packages)
    {
        delete package;
    }
    _packages.clear();
}

void Datasheet::pinSearch(int numPage)
{
    QList<Box> numbers;
    QList<Box> labels;
    QList<Box> proc_labels;
    QList<Box> pack_labels;
    QList<Pin> pins;
    QList<Package*> packages;

    qDebug()<<"+page: "<<numPage;
    if(_doc==NULL) {qDebug()<<"can not open"; return;}
    if(_doc->numPages()<1) {qDebug()<<"no page"; return;}

    Poppler::Page *page=_doc->page(numPage);
    //QRectF posPinDiagram=page->search("Pin Diagrams", Poppler::Page::CaseInsensitive).first();

    bool prev=false;
    Box box;
    foreach (TextBox *textBox, page->textList())
    {
        bool okNumber;
        //if(textBox->boundingBox().top()<posPinDiagram.top()) continue;
        if(textBox->text().startsWith("•")) textBox->text().mid(1).toInt(&okNumber);
        else textBox->text().toInt(&okNumber);
        if(textBox->nextWord()!=NULL && okNumber==false)
        {
            box.text = box.text+textBox->text();
            box.pos = textBox->boundingBox().united(box.pos);
            prev=true;
        }
        else
        {
            if(prev)
            {
                box.text = box.text+textBox->text();
                box.pos = textBox->boundingBox().united(box.pos);
            }
            else
            {
                if(textBox->text().startsWith("•")) box.text = textBox->text().mid(1);
                else box.text = textBox->text();
                box.pos = textBox->boundingBox();
                prev=false;
                if(okNumber)
                {
                    numbers.push_back(box);
                }
            }

            if(!okNumber || prev)
            {
                if(box.text.contains("Note",Qt::CaseInsensitive))
                {
                    // none
                }
                else if(box.text.startsWith("PIC",Qt::CaseInsensitive) ||
                        box.text.startsWith("DSPIC",Qt::CaseInsensitive) ||
                        box.text.startsWith("IS",Qt::CaseInsensitive) ||
                        box.text.startsWith("RX",Qt::CaseInsensitive))
                {
                    proc_labels.push_back(box);
                    qDebug()<<"proc_labels"<<box.text;
                }
                else if(box.text.contains("DIP",Qt::CaseInsensitive) ||
                        box.text.contains("SOIC",Qt::CaseInsensitive) ||
                        box.text.contains("BGA",Qt::CaseInsensitive) ||
                        box.text.contains("TQFP",Qt::CaseInsensitive) ||
                        box.text.contains("LQP",Qt::CaseInsensitive) ||
                        box.text.contains("LQFP",Qt::CaseInsensitive) ||
                        box.text.contains("LGA",Qt::CaseInsensitive))
                {
                    pack_labels.push_back(box);
                    qDebug()<<"pack_labels"<<box.text;
                }
                else if(box.text.size()>10 && !box.text.contains("/"))
                {
                    // none
                }
                else
                {
                    labels.push_back(box);
                }
            }
            prev=false;
            box.text="";
            box.pos=QRectF();
        }
    }

    // pairing label and number to pin
    foreach (Box number, numbers)
    {
        Pin pin;
        qreal dist=999999999999;
        QPointF center=number.pos.center();
        Box assoc;
        int near=0;
        for(int i=0;i<labels.count();i++)
        {
            Box label=labels[i];
            if(!isAlign(label, number)) continue;
            qreal newdist=(center-label.pos.center()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
            newdist=(center-label.pos.bottomLeft()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
            newdist=(center-label.pos.topRight()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
            newdist=(center-label.pos.bottomRight()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
            newdist=(center-label.pos.topLeft()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
        }
        assoc=labels[near];
        //labels.removeAt(near);

        pin.name=assoc.text.remove(QRegExp("\\([0-9]+\\)"));
        pin.pin=number.text.toInt();
        pin.pos=number.pos.united(assoc.pos);
        pin.numPos=number.pos;
        pins.push_back(pin);
    }

    // painring pin to find package
    foreach (Pin pin, pins)
    {
        if(pin.pin==1)
        {
            Package *package;
            package = new Package();
            package->pins.push_back(pin);
            packages.push_back(package);
        }
    }
    for(int pinNumber=2;pinNumber<200;pinNumber++)
    {
        for(int i=0;i<pins.count();i++)
        {
            Pin pin=pins.at(i);
            if(pin.pin==pinNumber)
            {
                Package *nearPackage=NULL;
                qreal dist=99999999999999;
                QPointF center=pin.numPos.center();
                foreach(Package *package, packages)
                {
                    Pin lastpin=package->pins.last();
                    //qDebug()<<dist<<pin.name<<pin.pin<<pin.pos<<lastpin.name;
                    if(lastpin.pin==pin.pin || lastpin.pin+4<pin.pin) continue;
                    qreal newdist=(center-(lastpin.numPos.bottomLeft())).manhattanLength();
                    if(newdist<dist)
                    {
                        dist=newdist;
                        nearPackage=package;
                    }
                    newdist=(center-lastpin.numPos.topRight()).manhattanLength();
                    if(newdist<dist)
                    {
                        dist=newdist;
                        nearPackage=package;
                    }
                    newdist=(center-lastpin.numPos.bottomRight()).manhattanLength();
                    if(newdist<dist)
                    {
                        dist=newdist;
                        nearPackage=package;
                    }
                    newdist=(center-lastpin.numPos.topLeft()).manhattanLength();
                    if(newdist<dist)
                    {
                        dist=newdist;
                        nearPackage=package;
                    }
                }
                if(nearPackage!=NULL)
                {
                    nearPackage->pins.push_back(pin);
                }
            }
        }
    }

    // ajust size of package
    foreach(Package *package, packages)
    {
        QRectF rect;
        QRectF rectNum;
        foreach (Pin pin, package->pins)
        {
            rect = rect.united(pin.pos);
            rectNum = rectNum.united(pin.numPos);
        }
        package->rect = rect;
        package->rectNum = rectNum;
    }

    int pac=0;
    float res=8;
    int dec=5;
    //_package.clear();
    foreach(Package *package, packages)
    {
        if(package->pins.count()<5)
        {
            delete package;
            continue;
        }
        QRectF rect=package->rect;
        rect.adjust(-dec,-dec,dec,dec);
        package->image = _doc->page(numPage)->renderToImage(72.0*res, 72.0*res,
                                                   rect.x()*res,
                                                   rect.y()*res,
                                                   rect.width()*res,
                                                   rect.height()*res);
        QPainter painter(&package->image);
        qDebug()<<"==============================";
        pac++;
        QFile file(QString("img/p%1_pack%2.txt").arg(numPage+1).arg(pac));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        qDebug()<<QString("img/p%1_pack%2.txt").arg(numPage+1).arg(pac);
        QTextStream textStream(&file);

        textStream<<"Proc: ";
        foreach (Box proc, proc_labels)
        {
            textStream<<proc.text<<"-";
        }
        textStream<<endl<<"Package: ";
        foreach (Box pack, pack_labels)
        {
            textStream<<pack.text<<"-";
        }
        textStream<<endl;

        painter.setPen(QPen(Qt::yellow,5));
        foreach (Box number, numbers)
        {
            painter.drawRect(QRect((number.pos.topLeft()-rect.topLeft()).toPoint()*res, number.pos.size().toSize()*res));
        }
        painter.setPen(QPen(Qt::blue,5));
        foreach (Box label, labels)
        {
            painter.drawRect(QRect((label.pos.topLeft()-rect.topLeft()).toPoint()*res, label.pos.size().toSize()*res));
        }
        painter.setPen(QPen(Qt::red,5));
        foreach (Pin pin, package->pins)
        {
            textStream<<pin.pin<<"\t"<<pin.name<<endl;
            painter.drawRect(QRect((pin.pos.topLeft()-rect.topLeft()).toPoint()*res, pin.pos.size().toSize()*res));
        }
        package->image.save(QString("img/p%1_pack%2.png").arg(numPage+1).arg(pac));
        file.close();
        package->name=QString("Package p.%1 pack.%2").arg(numPage+1).arg(pac);
        _packages.push_back(package);
        KicadExport kicad;
        kicad.exportPack(package, QString("img/p%1_pack%2.lib").arg(numPage+1).arg(pac));
        //qDebug()<<_packages.count();
    }
}

void Datasheet::pinSearchBGA(int numPage)
{
    QList<Box> numbers;
    QList<Box> labels;
    QList<Box> proc_labels;
    QList<Box> pack_labels;
    QList<Pin> pins;
    QList<Package*> packages;

    qDebug()<<"+pageBGA: "<<numPage;
    if(_doc==NULL) {qDebug()<<"can not open"; return;}
    if(_doc->numPages()<1) {qDebug()<<"no page"; return;}

    Poppler::Page *page=_doc->page(numPage);

    bool prev=false;
    Box box;
    foreach (TextBox *textBox, page->textList())
    {
        bool okNumber;
        textBox->text().toInt(&okNumber);
        if(textBox->nextWord()!=NULL && okNumber==false)
        {
            box.text = box.text+textBox->text();
            box.pos = textBox->boundingBox().united(box.pos);
            prev=true;
        }
        else
        {
            if(prev)
            {
                box.text = box.text+textBox->text();
                box.pos = textBox->boundingBox().united(box.pos);
            }
            else
            {
                if(textBox->text().startsWith("•")) box.text = textBox->text().mid(1);
                else box.text = textBox->text();
                box.pos = textBox->boundingBox();
                prev=false;
                if(okNumber)
                {
                    numbers.push_back(box);
                }
            }

            if(!okNumber || prev)
            {
                if(box.text.contains("Note",Qt::CaseInsensitive))
                {
                    // none
                }
                else if(box.text.startsWith("PIC",Qt::CaseInsensitive) ||
                        box.text.startsWith("DSPIC",Qt::CaseInsensitive) ||
                        box.text.startsWith("RX",Qt::CaseInsensitive))
                {
                    proc_labels.push_back(box);
                    qDebug()<<"proc_labels"<<box.text;
                }
                else if(box.text.contains("DIP",Qt::CaseInsensitive) ||
                        box.text.contains("SOIC",Qt::CaseInsensitive) ||
                        box.text.contains("BGA",Qt::CaseInsensitive) ||
                        box.text.contains("TQFP",Qt::CaseInsensitive) ||
                        box.text.contains("LQP",Qt::CaseInsensitive) ||
                        box.text.contains("LQFP",Qt::CaseInsensitive) ||
                        box.text.contains("LGA",Qt::CaseInsensitive))
                {
                    pack_labels.push_back(box);
                    qDebug()<<"pack_labels"<<box.text;
                }
                else if(box.text.size()>10 && !box.text.contains("/"))
                {
                    // none
                }
                else
                {
                    labels.push_back(box);
                }
            }
            prev=false;
            box.text="";
            box.pos=QRectF();
        }
    }

    // pairing label and number to pin
    foreach (Box number, numbers)
    {
        Pin pin;
        qreal dist=999999999999;
        QPointF center=number.pos.center();
        Box assoc;
        int near=0;
        for(int i=0;i<labels.count();i++)
        {
            Box label=labels[i];
            if(!isAlign(label, number)) continue;
            qreal newdist=(center-label.pos.center()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
            newdist=(center-label.pos.bottomLeft()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
            newdist=(center-label.pos.topRight()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
            newdist=(center-label.pos.bottomRight()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
            newdist=(center-label.pos.topLeft()).manhattanLength();
            if(newdist<dist)
            {
                dist=newdist;
                near=i;
            }
        }
        assoc=labels[near];
        //labels.removeAt(near);

        pin.name=assoc.text.remove(QRegExp("\\([0-9]+\\)"));
        pin.pin=number.text.toInt();
        pin.pos=number.pos.united(assoc.pos);
        pin.numPos=number.pos;
        pins.push_back(pin);
    }

    // painring pin to find package
    foreach (Pin pin, pins)
    {
        if(pin.pin==1)
        {
            Package *package;
            package = new Package();
            package->pins.push_back(pin);
            packages.push_back(package);
        }
    }
    for(int pinNumber=2;pinNumber<200;pinNumber++)
    {
        for(int i=0;i<pins.count();i++)
        {
            Pin pin=pins.at(i);
            if(pin.pin==pinNumber)
            {
                Package *nearPackage=NULL;
                qreal dist=99999999999999;
                QPointF center=pin.numPos.center();
                foreach(Package *package, packages)
                {
                    Pin lastpin=package->pins.last();
                    //qDebug()<<dist<<pin.name<<pin.pin<<pin.pos<<lastpin.name;
                    if(lastpin.pin==pin.pin || lastpin.pin+4<pin.pin) continue;
                    qreal newdist=(center-(lastpin.numPos.bottomLeft())).manhattanLength();
                    if(newdist<dist)
                    {
                        dist=newdist;
                        nearPackage=package;
                    }
                    newdist=(center-lastpin.numPos.topRight()).manhattanLength();
                    if(newdist<dist)
                    {
                        dist=newdist;
                        nearPackage=package;
                    }
                    newdist=(center-lastpin.numPos.bottomRight()).manhattanLength();
                    if(newdist<dist)
                    {
                        dist=newdist;
                        nearPackage=package;
                    }
                    newdist=(center-lastpin.numPos.topLeft()).manhattanLength();
                    if(newdist<dist)
                    {
                        dist=newdist;
                        nearPackage=package;
                    }
                }
                if(nearPackage!=NULL)
                {
                    nearPackage->pins.push_back(pin);
                }
            }
        }
    }

    // ajust size of package
    foreach(Package *package, packages)
    {
        QRectF rect;
        QRectF rectNum;
        foreach (Pin pin, package->pins)
        {
            rect = rect.united(pin.pos);
            rectNum = rectNum.united(pin.numPos);
        }
        package->rect = rect;
        package->rectNum = rectNum;
    }

    int pac=0;
    float res=8;
    int dec=5;
    //_package.clear();
    foreach(Package *package, packages)
    {
        if(package->pins.count()<5)
        {
            delete package;
            continue;
        }
        QRectF rect=package->rect;
        rect.adjust(-dec,-dec,dec,dec);
        package->image = _doc->page(numPage)->renderToImage(72.0*res, 72.0*res,
                                                   rect.x()*res,
                                                   rect.y()*res,
                                                   rect.width()*res,
                                                   rect.height()*res);
        QPainter painter(&package->image);
        qDebug()<<"==============================";
        pac++;
        QFile file(QString("img/p%1_pack%2.txt").arg(numPage+1).arg(pac));
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream textStream(&file);

        textStream<<"Proc: ";
        foreach (Box proc, proc_labels)
        {
            textStream<<proc.text<<"-";
        }
        textStream<<endl<<"Package: ";
        foreach (Box pack, pack_labels)
        {
            textStream<<pack.text<<"-";
        }
        textStream<<endl;

        painter.setPen(QPen(Qt::yellow,5));
        foreach (Box number, numbers)
        {
            painter.drawRect(QRect((number.pos.topLeft()-rect.topLeft()).toPoint()*res, number.pos.size().toSize()*res));
        }
        painter.setPen(QPen(Qt::blue,5));
        foreach (Box label, labels)
        {
            painter.drawRect(QRect((label.pos.topLeft()-rect.topLeft()).toPoint()*res, label.pos.size().toSize()*res));
        }
        painter.setPen(QPen(Qt::red,5));
        foreach (Pin pin, package->pins)
        {
            textStream<<pin.pin<<"\t"<<pin.name<<endl;
            painter.drawRect(QRect((pin.pos.topLeft()-rect.topLeft()).toPoint()*res, pin.pos.size().toSize()*res));
        }
        package->image.save(QString("img/p%1_pack%2.png").arg(numPage+1).arg(pac));
        file.close();
        package->name=QString("Package p.%1 pack.%2").arg(numPage+1).arg(pac);
        _packages.push_back(package);
        //qDebug()<<_packages.count();
    }
}

int Datasheet::pagePinDiagram(int pageStart, bool *bgaStyle)
{
    if(_doc==NULL) {qDebug()<<"can not open"; return -1;}
    if(_doc->numPages()<1) {qDebug()<<"no page"; return -1;}

    if(pageStart>=_doc->numPages()) return -1;
    for(int i=pageStart;i<_doc->numPages();i++)
    {
        QList<QRectF> result;
        Poppler::Page *page=_doc->page(i);
        if(page==NULL) continue;
        result=page->search("vss", Poppler::Page::CaseInsensitive);         // All
        if(result.isEmpty())
        {
            result=page->search("vdd", Poppler::Page::CaseInsensitive);         // All
            if(result.isEmpty())
            {
                result=page->search("gnd", Poppler::Page::CaseInsensitive);         // All
                if(result.isEmpty()) continue;
            }
        }
        result=page->search("BGA", Poppler::Page::CaseInsensitive);         // All
        //if(result.isEmpty()) *bgaStyle=false; else *bgaStyle=true;
        *bgaStyle=false;
        result=page->search("Pin Diagram", Poppler::Page::CaseInsensitive);         // Microchip
        if(!result.isEmpty()) return i;
        result=page->search("Device Pin Tables", Poppler::Page::CaseInsensitive);         // Microchip
        if(!result.isEmpty()) return i;
        result=page->search("Pin Diagram", Poppler::Page::CaseInsensitive);         // Microchip
        if(!result.isEmpty()) return i;
        result=page->search("PIN DESCRIPTION", Poppler::Page::CaseInsensitive);         // Microchip
        if(!result.isEmpty()) return i;
        result=page->search("PIN NAMES", Poppler::Page::CaseInsensitive);         // Microchip
        if(!result.isEmpty()) return i;
        result=page->search("Pin Assignment", Poppler::Page::CaseInsensitive);      // Renesas
        if(!result.isEmpty()) return i;
        result=page->search("PIN CONFIGURATION", Poppler::Page::CaseInsensitive);   // Maxim
        if(!result.isEmpty()) return i;
        result=page->search("PACKAGE", Poppler::Page::CaseInsensitive);   // Linear tech
        if(!result.isEmpty()) return i;
        result=page->search("Interface pin description", Poppler::Page::CaseInsensitive);   // Divers
        if(!result.isEmpty()) return i;
    }
    return -1;
}

void Datasheet::analyse()
{
    int page=-1;
    bool bgaStyle;
    while((page=pagePinDiagram(page+1, &bgaStyle))!=-1)
    {
        if(bgaStyle) pinSearchBGA(page);
        else pinSearch(page);
    }
}

bool Datasheet::isAlign(Box &label, Box &number)
{
    if(label.pos.width()>label.pos.height())    // Horizontal
    {
        int marge=label.pos.height();
        if(label.pos.top()-marge<number.pos.top() && label.pos.bottom()+marge>number.pos.bottom()) return true;
        else return false;
    }
    else                                        // Vertical
    {
        int marge=label.pos.width();
        if(label.pos.left()-marge<number.pos.left() && label.pos.right()+marge>number.pos.right()) return true;
        else return false;
    }
}

const QList<Package *> &Datasheet::packages() const
{
    return _packages;
}

Package::Package()
{

}

Package::~Package()
{

}
