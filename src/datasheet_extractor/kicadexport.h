#ifndef KICADEXPORT_H
#define KICADEXPORT_H

#include "datasheet.h"
#include <QTextStream>

class KicadExport
{
public:
    KicadExport();

    bool exportPack(DatasheetPackage *pack, QString fileName);

private:
    QTextStream _out;

    void exportPin(DatasheetPin pin, int x, int y, int orientation);
};

#endif // KICADEXPORT_H
