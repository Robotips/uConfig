/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include <QApplication>
#include <QCommandLineParser>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QScreen>
#include <QTextStream>
#include <utility>

#include <pdf_extract/datasheet.h>

#include <kicad/model/lib.h>
#include <kicad/viewer/componentviewer.h>

#include <kicad/pinruler/pinruler.h>
#include <kicad/pinruler/rulesparser.h>

#include <kicad/schematicsimport/textimporter.h>

QTextStream out(stdout);

void processFilePdf(QString file, Lib *lib, bool debug)
{
    Datasheet datasheet;
    datasheet.setDebugEnabled(debug);
    bool opened = datasheet.open(std::move(file));
    if (!opened)
    {
        out << "error (2): input file cannot be opened" << Qt::endl;
        exit(2);
    }
    datasheet.analyse();
    for (DatasheetPackage *package : datasheet.packages())
    {
        Component *component = package->toComponent();
        lib->addComponent(component);
    }
}

void processFileLib(const QString &file, Lib *lib)
{
    if (!lib->readFrom(file))
    {
        out << "error (2): input file cannot be opened" << Qt::endl;
        exit(2);
    }
}

void processFileCSV(const QString &file, Lib *lib)
{
    TextImporter imp;
    if (!imp.import(file))
    {
        out << "error (2): input file cannot be opened" << Qt::endl;
        exit(2);
    }

    for (Component *component : qAsConst(imp.components()))
    {
        lib->addComponent(component);
    }
}

enum UConfigSource
{
    FromPdf,
    FromLib,
    FromCSV
} source;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("uConfig");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "uConfig command line interface.")
                                     + QString("\nuconfig datasheet.pdf -o lib1.lib [-r rule.kss] [-d]"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", QCoreApplication::translate("main", "Source file to extract pins (pdf)."), "file");

    QCommandLineOption outOption(QStringList() << "o"
                                               << "out",
                                 QCoreApplication::translate("main", "Output file with pin list."),
                                 "out");
    parser.addOption(outOption);

    QCommandLineOption debugOption(QStringList() << "d"
                                                 << "debug",
                                   QCoreApplication::translate("main", "Debug option to view intermediate steps."));
    parser.addOption(debugOption);

    QCommandLineOption kssOption(QStringList() << "r"
                                               << "rule",
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
        if (!ruleFile.endsWith(".kss", Qt::CaseInsensitive))
        {
            ruleFile.append(".kss");
        }
        if (!QFileInfo::exists(ruleFile))
        {
            QString binPath = QCoreApplication::applicationDirPath() + "/";
            if (QFileInfo::exists(binPath + "../rules/" + ruleFile))
            {
                ruleFile = binPath + "../rules/" + ruleFile;
            }
            else
            {
                out << "error (3): ruleFile '" << parser.value("rule") << "' does not exist" << Qt::endl;
                return -3;
            }
        }
        RulesParser ruleParser(ruleFile);
        if (!ruleParser.parse(&ruleSet))
        {
            out << "error (4): error when parsing ruleFile '" << parser.value("rule") << "' at line " << ruleParser.errorLine() << Qt::endl;
            return -4;
        }
        ruler.setRuleSet(&ruleSet);
    }

    // input file
    const QStringList files = parser.positionalArguments();
    if (files.isEmpty())
    {
        out << "error (1): input file is needed" << Qt::endl;
        parser.showHelp(-1);
    }
    const QString &file = files.at(0);

    // output file
    QString outFile = parser.value("out");
    if (outFile.isEmpty())
    {
        outFile = QFileInfo(file).baseName() + ".lib";
    }

    // creates lib model and fill it
    qint64 d = QDateTime::currentMSecsSinceEpoch();
    Lib lib;
    out << "> " << file << Qt::endl;
    if (file.endsWith(".pdf", Qt::CaseInsensitive))
    {
        processFilePdf(file, &lib, parser.isSet(debugOption));
        source = FromPdf;
    }
    else if (file.endsWith(".lib", Qt::CaseInsensitive))
    {
        processFileLib(file, &lib);
        source = FromLib;
    }
    else if (file.endsWith(".csv", Qt::CaseInsensitive) || file.endsWith(".txt", Qt::CaseInsensitive))
    {
        processFileCSV(file, &lib);
        source = FromCSV;
    }
    else
    {
        out << "error (2): input file cannot be opened" << Qt::endl;
        exit(2);
    }
    out << lib.componentsCount() << " packages extracted, saved in " << outFile << Qt::endl;

    // apply pinruler if needed
    for (Component *component : lib.components())
    {
        if (ruleFile.isEmpty())
        {
            if (source == FromPdf)
            {
                component->reorganizeToPackageStyle();
            }
        }
        else
        {
            ruler.organize(component);
        }
    }

    out << "Elapsed time: " << QDateTime::currentMSecsSinceEpoch() - d << "ms" << Qt::endl;
    lib.saveTo(outFile);

    return 0;
}
