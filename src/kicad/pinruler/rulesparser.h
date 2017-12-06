#ifndef RULESPARSER_H
#define RULESPARSER_H

#include "kicad_global.h"

#include <QString>
#include <QStringList>

#include "rulesset.h"

class KICAD_EXPORT RulesParser
{
public:
    RulesParser(const QString &fileName=QString());

    void setData(const QString &data);
    bool parse(RulesSet *ruleSet);

    int errorLine() const;

protected:
    void skipSpace();
    bool skipComments();
    void skipSpaceAndComments();
    QString getSelector();
    //QStringList getSelectors();
    QString getPropertyName();
    QString getPropertyValue();

private:
    QString _fileName;
    QString _data;
    int _id;
    int _line;
    int _errorLine;
};

#endif // RULESPARSER_H
