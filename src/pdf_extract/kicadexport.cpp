#include "kicadexport.h"

#include <QFile>
#include <QDateTime>

KicadExport::KicadExport()
{
}

bool KicadExport::exportPack(DatasheetPackage *pack, QString fileName)
{
    QFile output(fileName);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    _out.setDevice(&output);

    // ==== header ====
    _out << "EESchema-LIBRARY Version 2.3  "
         << QDateTime::currentDateTime().toString("dd/MM/yyyy-hh:mm:ss")
         << endl;
    _out << endl;

    // ===== defs =====
    _out << "DEF " << pack->name << " U 0 40 Y Y 1 F N" << endl;
    _out << "F0 \"U\" 0 0 50 H V C CNN" << endl;
    _out << "F1 \"" << pack->name << "\" 0 0 50 H V C CNN" << endl;

    // ===== pins =====
    _out << "DRAW" << endl;
    _out << "S -800 1250 800 -1250 0 1 0 N" << endl;  // rect

    int pinCount = 0;
    int y = -100 * (pack->pins.count() / 4), x = -1100, orientation = 0;
    foreach (DatasheetPin pin, pack->pins)
    {
        exportPin(pin, x, y, orientation);
        pinCount++;

        if (pinCount == pack->pins.count() / 2)
        {
            orientation = 2;
            x = -x;
            y = -100 * (pack->pins.count() / 4);
        }
        else
        {
            y += 100;
        }
    }

    _out << "ENDDRAW" << endl;
    _out << "ENDDEF" << endl;
    output.close();
    return true;
}

void KicadExport::exportPin(DatasheetPin pin, int x, int y, int orientation)
{
    QString orientation_str;
    switch (orientation)
    {
    case 0:
        orientation_str = "R";
        break;
    case 1:
        orientation_str = "U";
        break;
    case 2:
        orientation_str = "L";
        break;
    case 3:
        orientation_str = "D";
        break;
    default:
        break;
    }

    _out << "X " << pin.name << " " << pin.pin << " " << x << " " << y
         << " 300 " << orientation_str << " 50 50 1 1 I" << endl;
}
