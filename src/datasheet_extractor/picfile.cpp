#include "picfile.h"

#include <QDebug>
#include <QByteArray>
#include <QDomDocument>

PicFile::PicFile()
{
}

bool PicFile::open(QString file)
{
    _file.setFileName(file);
    if (!_file.open(QIODevice::ReadOnly))
        return false;

    /*QXmlStreamReader reader(&_file);
    while(reader.readNextStartElement())
    {
        qDebug()<<reader.name();
        foreach(QXmlStreamAttribute at, reader.attributes())
        {
            qDebug()<<'\t'<<at.name();
        }
        reader.skipCurrentElement();
    }*/
    /*QFile out("out.txt");
    out.open(QIODevice::WriteOnly);
    QByteArray bite = _file.readAll();
    QByteArray decode = qUncompress(bite);
    out.write(decode);*/

    QDomDocument doc;
    doc.setContent(&_file);
    _file.close();
    QDomElement root = doc.documentElement();

    _dsid = root.attribute("edc:dsid").toInt();
    _psid = root.attribute("edc:psid").toInt();

    /*QDomElement child=root.firstChild().toElement();
    while(!child.isNull())
    {
        qDebug()<<child.nodeName().remove("edc:");
        QDomElement grandChild=child.firstChild().toElement();
        while(!grandChild.isNull())
        {
            qDebug()<<'\t'<<grandChild.nodeName().remove("edc:");
            grandChild = grandChild.nextSibling().toElement();
        }
        child = child.nextSibling().toElement();
    }*/
    return true;
}

int PicFile::dsid()
{
    return _dsid;
}

int PicFile::psid()
{
    return _psid;
}
