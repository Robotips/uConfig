#ifndef DATASHEET_H
#define DATASHEET_H

#include <poppler-qt5.h>
#include <poppler-form.h>
#include <QImage>

struct Box
{
    QString text;
    QRectF pos;
};

struct Pin
{
    QString name;
    int pin;
    QRectF pos;
    QRectF numPos;
};

class Package
{
public:
    Package();
    ~Package();

public:
    QString name;
    QRectF rect;
    QRectF rectNum;
    QList<Pin> pins;
    QImage image;
};

class Datasheet
{
public:
    Datasheet();
    ~Datasheet();

    void open(int dsid);
    void open(QString fileName);
    void close();

    void analyse();

    const QList<Package *> &packages()const;

private:
    int pagePinDiagram(int pageStart, bool *bgaStyle);

    void pinSearchBGA(int numPage);
    void pinSearch(int numPage);

    Poppler::Document *_doc;

    bool isAlign(Box &label, Box &number);
    QList<Package*> _packages;
};

#endif // DATASHEET_H
