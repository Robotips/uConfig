#include "datasheetdownloader.h"

#include <QDebug>
#include <QFile>
#include <QDir>

DatasheetDownloader::DatasheetDownloader(QObject *parent) :
    QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
    connect(_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
}

DatasheetDownloader::~DatasheetDownloader()
{
    delete _manager ;
}

void DatasheetDownloader::sort()
{
    //qDebug()<<
    QDir dir("edc/doc");
    QString previousdoc;
    foreach(QString doc, dir.entryList(QStringList("*.pdf"), QDir::Files, QDir::Name))
    {
        if(doc.left(5)==previousdoc.left(5)) QFile::rename("edc/doc/"+previousdoc,"edc/doc/old/"+previousdoc);
        previousdoc=doc;
    }
}

void DatasheetDownloader::download(int dsid)
{
    /*_manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1a.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1b.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1c.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1d.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1e.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1f.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1g.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1h.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1i.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1j.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1k.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1l.pdf").arg(dsid))));
    _manager->get(QNetworkRequest(QUrl(QString("http://ww1.microchip.com/downloads/en/DeviceDoc/%1.pdf").arg(dsid))));*/
}

void DatasheetDownloader::replyFinished(QNetworkReply *reply)
{
    if(reply->bytesAvailable()>30)
    {
        qDebug()<<reply->url().toString();
        //qDebug()<<reply->read(100);
        QFile file("edc/doc/"+reply->url().toString().section('/', -1));
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
    }
}
