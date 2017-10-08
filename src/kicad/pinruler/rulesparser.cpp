#include "rulesparser.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

#include <QRegularExpression>

RulesParser::RulesParser(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);
    _data = stream.readAll();
    file.close();
    _id = 0;
    _line = 1;

    parse();
}

bool RulesParser::parse()
{
    skipSpaceAndComments();
    int lineSelector;
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

        QString propertyName = getPropertyName();
        QString propertyValue;
        while (!propertyName.isEmpty())
        {
            propertyValue = getPropertyValue();
            qDebug()<<propertyName<<"="<<propertyValue;
            skipSpaceAndComments();
            propertyName = getPropertyName();
        }
        skipSpaceAndComments();
        if (_data[_id] == '}') // end of rule
        {
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
    while ((*pt == '\n' || *pt == '\r' || *pt == '\t' || *pt == ' ') && *pt != 0)
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
    while (*pt != 0)
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
    QRegularExpression rule("(\\.?[a-zA-Z][a-zA-Z0-9\\+\\-\\[\\]\\(\\)\\_\\|]*)", QRegularExpression::MultilineOption
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
    QRegularExpression rule("\"?([a-zA-Z0-9\\_\\-\\+]*)\"?;?", QRegularExpression::MultilineOption
                                    | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch ruleMath = rule.match(_data.mid(_id));
    if (ruleMath.hasMatch() && ruleMath.capturedStart() != 0)
        return QString();
    _id += ruleMath.capturedEnd();
    return  ruleMath.captured(1);
}
