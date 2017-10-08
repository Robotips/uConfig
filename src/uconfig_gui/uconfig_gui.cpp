#include <QDebug>
#include <QApplication>

#include "importer/pinlistimporter.h"

int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    QApplication app(argc, argv);

    PinListImporter importer;
    importer.show();

    return app.exec();
}
