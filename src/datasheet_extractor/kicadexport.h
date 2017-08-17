#ifndef KICADEXPORT_H
#define KICADEXPORT_H

#include "datasheet.h"
#include <QTextStream>

class KicadExport
{
public:
    KicadExport();

    bool exportPack(Package *pack, QString fileName);

private:
    QTextStream _out;

    void exportPin(Pin pin, int x, int y, int orientation);
};

#endif // KICADEXPORT_H
