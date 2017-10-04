#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QScreen>
#include <QCommandLineParser>
#include <QTextStream>
#include <QFileInfo>

#include "../pdf_extract/datasheet.h"

#include "../kicad/viewer/componentviewer.h"
#include "../kicad/lib.h"

int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    QApplication app(argc, argv);
    QApplication::setApplicationName("uconfig");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "Source file to extract pins (pdf).", "file");

    QCommandLineOption outOption(QStringList() << "o" << "out",
                                 "Outpout file with pin list", "out");
    parser.addOption(outOption);

    /* TODO implement debug option
    QCommandLineOption debugOption(QStringList() << "d" << "debug",
                                   "Debug option to view intermediate steps");
    parser.addOption(debugOption);
    */

    /* TODO implement range page option
    QCommandLineOption rangeOption(QStringList() << "p" << "page",
                                  "Range of page to search into, defaut is the full document",
                                  "range");
    parser.addOption(rangeOption);
    */

    parser.process(app);

    const QStringList files = parser.positionalArguments();
    if (files.isEmpty())
    {
        out << "error (1): input file is needed" << endl;
        parser.showHelp(-1);
    }
    const QString &file = files.at(0);

    Datasheet datasheet;
    bool opened = datasheet.open(file);
    if (!opened)
    {
        out << "error (2): input file cannot be opened" << endl;
        return -2;
    }

    QString outFile = parser.value("out");
    if (outFile.isEmpty())
        outFile = QFileInfo(file).baseName() + ".lib";

    out << "> " << file << endl;
    qint64 d = QDateTime::currentMSecsSinceEpoch();
    datasheet.analyse();
    Lib lib;
    foreach(DatasheetPackage *package, datasheet.packages())
    {
        Component *component = package->toComponent();
        component->reorganizeToPackageStyle();
        lib.addComponent(component);
    }
    out << datasheet.packages().count() << " packages extracted, saved in " << outFile << endl;
    out << "Elapsed time: " << QDateTime::currentMSecsSinceEpoch() - d << "ms" << endl;
    lib.saveTo(outFile);

    return 0;
}
