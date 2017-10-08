#ifndef RULESPARSER_H
#define RULESPARSER_H

#include "kicad_global.h"

#include <QString>
#include <QStringList>

class KICAD_EXPORT RulesParser
{
public:
    RulesParser(const QString &fileName);

protected:
    bool parse();

    void skipSpace();
    bool skipComments();
    void skipSpaceAndComments();
    QString getSelector();
    //QStringList getSelectors();
    QString getPropertyName();
    QString getPropertyValue();

private:
    QString _data;
    int _id;
    int _line;
};

#endif // RULESPARSER_H
