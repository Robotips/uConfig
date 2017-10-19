#include "rulesparser.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <QRegularExpression>

RulesParser::RulesParser(const QString &fileName)
{
    _fileName = fileName;
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);
    _data = stream.readAll();
    file.close();
}

bool RulesParser::parse(RulesSet *ruleSet)
{
    _id = 0;
    _line = 1;
    _errorLine = -1;
    Rule *rule;

    skipSpaceAndComments();
    int lineSelector;
    QString selector = getSelector();
    while (!selector.isEmpty())
    {
        lineSelector = _line;
        skipSpaceAndComments();
        if (_data[_id] != '{') // start of rule
        {
            _errorLine = _line;
            return false; // error
        }
        //qDebug()<<"> "<<selector<<lineSelector;

        if (selector.startsWith('.'))
            rule = new ClassRule(selector.mid(1));
        else
            rule = new PinRule(selector);
        rule->setFile(_fileName);
        rule->setLine(lineSelector);
        _id ++;
        skipSpaceAndComments();

        QString propertyName = getPropertyName();
        QString propertyValue;
        while (!propertyName.isEmpty())
        {
            propertyValue = getPropertyValue();
            rule->setProperty(propertyName, propertyValue);
            //qDebug()<<propertyName<<"="<<propertyValue;
            skipSpaceAndComments();

            propertyName = getPropertyName();
        }
        skipSpaceAndComments();
        if (_data[_id] == '}') // end of rule
        {
            ruleSet->addRule(rule);
            _id++;
            skipSpaceAndComments();
        }
        selector = getSelector();
    }
    return true;
}

void RulesParser::skipSpace()
{
    const QChar *pt = _data.constData() + _id;
    while ((*pt == '\n' || *pt == '\r' || *pt == '\t' || *pt == ' ') && !pt->isNull())
    {
        if (*pt == '\n')
            _line++;
        _id++;
        pt++;
    }
}

bool RulesParser::skipComments()
{
    const QChar *pt = _data.constData() + _id;
    if (*pt != '/')
        return false;
    if (*(pt+1) != '*')
        return false;
    pt+=2;
    _id+=2;
    while (!pt->isNull())
    {
        if (*pt == '*')
        {
            if (*(pt+1) == '/')
            {
                pt+=2;
                _id+=2;
                return true;
            }
        }
        if (*pt == '\n')
            _line++;
        _id++;
        pt++;
    }
    return false;
}

void RulesParser::skipSpaceAndComments()
{
    skipSpace();
    while(skipComments())
        skipSpace();
}

QString RulesParser::getSelector()
{
    QRegularExpression rule("(\\.?[a-zA-Z\\(\\[][/a-zA-Z0-9\\+\\-\\[\\]\\(\\)\\_\\|\\\\\\*\\.\\^$\\?]*)", QRegularExpression::MultilineOption
                                    | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch ruleMath = rule.match(_data.mid(_id));
    if (ruleMath.hasMatch() && ruleMath.capturedStart() != 0)
        return QString();
    _id += ruleMath.capturedEnd();
    return  ruleMath.captured(1);
}

/*QStringList RulesParser::getSelectors()
{
    QStringList selectors;
    QString selector = getSelector();
    while (!selector.isEmpty())
    {
        lineSelector = _line;
        skipSpaceAndComments();
        if (_data[_id] != '{') // start of rule
        {
            qDebug()<<"error at line"<<_line;
            return false; // error
        }
        qDebug()<<"> "<<selector<<lineSelector;
        _id ++;
        skipSpaceAndComments();
    }
    return selectors;
}*/

QString RulesParser::getPropertyName()
{
    QRegularExpression rule("([a-zA-Z][a-zA-Z0-9\\_\\-]*)[\t ]*:[\t ]*", QRegularExpression::MultilineOption
                                    | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch ruleMath = rule.match(_data.mid(_id));
    if (ruleMath.hasMatch() && ruleMath.capturedStart() != 0)
        return QString();
    _id += ruleMath.capturedEnd();
    return  ruleMath.captured(1);
}

QString RulesParser::getPropertyValue()
{
    QRegularExpression rule("\"?([a-zA-Z0-9\\_\\-\\+\\\\\\[\\]\\(\\)\\.\\*]*)\"?;?", QRegularExpression::MultilineOption
                                    | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch ruleMath = rule.match(_data.mid(_id));
    if (ruleMath.hasMatch() && ruleMath.capturedStart() != 0)
        return QString();
    _id += ruleMath.capturedEnd();
    return  ruleMath.captured(1);
}

int RulesParser::errorLine() const
{
    return _errorLine;
}
