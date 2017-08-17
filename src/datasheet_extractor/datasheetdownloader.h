#ifndef DATASHEETDOWNLOADER_H
#define DATASHEETDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DatasheetDownloader : public QObject
{
    Q_OBJECT
public:
    DatasheetDownloader(QObject *parent = 0);
    ~DatasheetDownloader();

    void download(int dsid);

    void sort();
signals:

private slots:
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *_manager;

};

#endif // DATASHEETDOWNLOADER_H
