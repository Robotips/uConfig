#ifndef PICFILE_H
#define PICFILE_H

#include <QFile>
#include <QXmlStreamReader>

class PicFile
{
public:
    PicFile();

    bool open(QString file);

    int dsid();
    int psid();

private:
    QFile _file;

    int _dsid, _psid;
};

#endif  // PICFILE_H
