#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include <QScreen>
#include <QCommandLineParser>
#include <QTextStream>
#include <QFileInfo>

#include "../pdf_extract/datasheet.h"

#include "../kicad/viewer/componentviewer.h"
#include "../kicad/model/lib.h"

#include "../kicad/pinruler/pinruler.h"
#include "../kicad/pinruler/rulesparser.h"

QTextStream out(stdout);

void processFilePdf(QString file, Lib *lib, bool debug)
{
    Datasheet datasheet;
    datasheet.setDebugEnabled(debug);
    bool opened = datasheet.open(file);
    if (!opened)
    {
        out << "error (2): input file cannot be opened" << endl;
        exit(2);
    }
    datasheet.analyse();
    foreach(DatasheetPackage *package, datasheet.packages())
    {
        Component *component = package->toComponent();
        lib->addComponent(component);
    }
}

void processFileLib(QString file, Lib *lib)
{
    if (!lib->readFrom(file))
    {
        out << "error (2): input file cannot be opened" << endl;
        exit(2);
    }
}

enum UConfigSource {
    FromPdf,
    FromLib
} source;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("uConfig");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "uConfig command line interface.") +
                                                                 QString("\nuconfig datasheet.pdf -o lib1.lib [-r rule.kss] [-d]"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", QCoreApplication::translate("main", "Source file to extract pins (pdf)."), "file");

    QCommandLineOption outOption(QStringList() << "o" << "out",
                                 QCoreApplication::translate("main", "Output file with pin list."),
                                 "out");
    parser.addOption(outOption);

    QCommandLineOption debugOption(QStringList() << "d" << "debug",
                                   QCoreApplication::translate("main", "Debug option to view intermediate steps."));
    parser.addOption(debugOption);

    QCommandLineOption kssOption(QStringList() << "r" << "rule",
                                 QCoreApplication::translate("main", "KSS rule file to organize component."),
                                 "rule");
    parser.addOption(kssOption);

    /* TODO implement range page option
    QCommandLineOption rangeOption(QStringList() << "p" << "page",
                                  "Range of page to search into, defaut is the full document",
                                  "range");
    parser.addOption(rangeOption);
    */

    parser.process(app);

    // rule file
    QString ruleFile = parser.value("rule");
    PinRuler ruler;
    RulesSet ruleSet;
    if (!ruleFile.isEmpty())
    {
        if (!ruleFile.endsWith(".kss"))
            ruleFile.append(".kss");
        if (!QFileInfo(ruleFile).exists())
        {
            QString binPath = QFileInfo(QApplication::arguments()[0]).path() + "/";
            if (QFileInfo(binPath + "../rules/"+ruleFile).exists())
            {
                ruleFile = binPath + "../rules/"+ruleFile;
            }
            else
            {
                out << "error (3): ruleFile '" << parser.value("rule") << "' does not exist" << endl;
                return -3;
            }
        }
        RulesParser ruleParser(ruleFile);
        if (!ruleParser.parse(&ruleSet))
        {
            out << "error (4): error when parsing ruleFile '" << parser.value("rule") << "' at line " << ruleParser.errorLine() << endl;
            return -4;
        }
        ruler.setRuleSet(&ruleSet);
    }

    // input file
    const QStringList files = parser.positionalArguments();
    if (files.isEmpty())
    {
        out << "error (1): input file is needed" << endl;
        parser.showHelp(-1);
    }
    const QString &file = files.at(0);

    // output file
    QString outFile = parser.value("out");
    if (outFile.isEmpty())
        outFile = QFileInfo(file).baseName() + ".lib";

    // creates lib model and fill it
    qint64 d = QDateTime::currentMSecsSinceEpoch();
    Lib lib;
    out << "> " << file << endl;
    if (file.endsWith(".pdf"))
    {
        processFilePdf(file, &lib, parser.isSet(debugOption));
        source = FromPdf;
    }
    else if (file.endsWith(".lib"))
    {
        processFileLib(file, &lib);
        source = FromLib;
    }
    else
    {
        out << "error (2): input file cannot be opened" << endl;
        exit(2);
    }
    out << lib.componentsCount() << " packages extracted, saved in " << outFile << endl;

    // apply pinruler if needed
    foreach(Component *component, lib.components())
    {
        if (ruleFile.isEmpty())
        {
            if (source == FromPdf)
                component->reorganizeToPackageStyle();
        }
        else
        {
            ruler.organize(component);
        }
    }

    out << "Elapsed time: " << QDateTime::currentMSecsSinceEpoch() - d << "ms" << endl;
    lib.saveTo(outFile);

    return 0;
}
