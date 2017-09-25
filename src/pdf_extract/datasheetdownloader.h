#ifndef DATASHEETDOWNLOADER_H
#define DATASHEETDOWNLOADER_H

#include "pdf_extract_common.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DATASHEET_EXTRACTOR_EXPORT DatasheetDownloader : public QObject
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

#endif  // DATASHEETDOWNLOADER_H
