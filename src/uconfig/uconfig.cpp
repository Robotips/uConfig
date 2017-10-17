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

#include "../kicad/pinruler/pinruler.h"
#include "../kicad/pinruler/rulesparser.h"

int main(int argc, char *argv[])
{
    QTextStream out(stdout);
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("uconfig");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "Source file to extract pins (pdf).", "file");

    QCommandLineOption outOption(QStringList() << "o" << "out",
                                 "Outpout file with pin list",
                                 "out");
    parser.addOption(outOption);

    QCommandLineOption debugOption(QStringList() << "d" << "debug",
                                   "Debug option to view intermediate steps");
    parser.addOption(debugOption);

    QCommandLineOption kssOption(QStringList() << "r" << "rule",
                                 "KSS rule file to organize component",
                                 "rule");
    parser.addOption(kssOption);

    /* TODO implement range page option
    QCommandLineOption rangeOption(QStringList() << "p" << "page",
                                  "Range of page to search into, defaut is the full document",
                                  "range");
    parser.addOption(rangeOption);
    */

    parser.process(app);

    QString ruleFile = parser.value("rule");
    if (!ruleFile.isEmpty())
    {
        if (!ruleFile.endsWith(".kss"))
            ruleFile.append(".kss");
        if (!QFileInfo(ruleFile).exists())
        {
            if (QFileInfo("../rules/"+ruleFile).exists())
            {
                ruleFile = "../rules/"+ruleFile;
            }
            else
            {
                out << "error (3): ruleFile '" << parser.value("rule") << "' does not exist" << endl;
                return -3;
            }
        }
    }
    RulesSet ruleSet;
    PinRuler ruler;
    if (!ruleFile.isEmpty())
    {
        RulesParser ruleParser(ruleFile);
        if (!ruleParser.parse(&ruleSet))
        {
            out << "error (4): error when parsing ruleFile '" << parser.value("rule") << "' at line " << ruleParser.errorLine() << endl;
            return -4;
        }
        ruler.setRuleSet(&ruleSet);
    }

    const QStringList files = parser.positionalArguments();
    if (files.isEmpty())
    {
        out << "error (1): input file is needed" << endl;
        parser.showHelp(-1);
    }
    const QString &file = files.at(0);

    Datasheet datasheet;
    datasheet.setDebugEnabled(parser.isSet(debugOption));
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

        if (ruleFile.isEmpty())
            component->reorganizeToPackageStyle();
        else
            ruler.organize(component);
        lib.addComponent(component);
    }
    out << datasheet.packages().count() << " packages extracted, saved in " << outFile << endl;
    out << "Elapsed time: " << QDateTime::currentMSecsSinceEpoch() - d << "ms" << endl;
    lib.saveTo(outFile);

    return 0;
}
