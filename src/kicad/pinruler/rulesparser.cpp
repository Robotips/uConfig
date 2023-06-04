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

#include "rulesparser.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <QRegularExpression>

RulesParser::RulesParser(const QString &fileName)
{
    _errorLine = -1;
    _fileName = fileName;
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream stream(&file);
        _data = stream.readAll();
        file.close();
    }
}

void RulesParser::setData(const QString &data)
{
    _data = data;
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
        if (_data[_id] != '{')  // start of rule
        {
            _errorLine = _line;
            return false;  // error
        }

        if (selector.startsWith('.') && !selector.startsWith(".*"))
        {
            rule = new ClassRule(selector.mid(1));
        }
        else
        {
            rule = new PinRule(selector);
        }
        rule->setFile(_fileName);
        rule->setLine(lineSelector);
        _id++;
        skipSpaceAndComments();

        QString propertyName = getPropertyName();
        if (propertyName.isEmpty())
        {
            _errorLine = _line;
            delete rule;
            return false;  // error
        }
        QString propertyValue;
        while (!propertyName.isEmpty())
        {
            propertyValue = getPropertyValue();
            rule->setProperty(propertyName, propertyValue);
            // qDebug()<<propertyName<<"="<<propertyValue;
            skipSpaceAndComments();

            propertyName = getPropertyName();
        }
        skipSpaceAndComments();
        if (_data[_id] == '}')  // end of rule
        {
            ruleSet->addRule(rule);
            _id++;
            skipSpaceAndComments();
        }
        else
        {
            _errorLine = _line;
            delete rule;
            return false;  // error
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
        {
            _line++;
        }
        _id++;
        pt++;
    }
}

bool RulesParser::skipComments()
{
    const QChar *pt = _data.constData() + _id;
    if (*pt != '/')
    {
        return false;
    }
    if (*(pt + 1) != '*')
    {
        return false;
    }
    pt += 2;
    _id += 2;
    while (!pt->isNull())
    {
        if (*pt == '*')
        {
            if (*(pt + 1) == '/')
            {
                pt += 2;
                _id += 2;
                return true;
            }
        }
        if (*pt == '\n')
        {
            _line++;
        }
        _id++;
        pt++;
    }
    return false;
}

void RulesParser::skipSpaceAndComments()
{
    skipSpace();
    while (skipComments())
    {
        skipSpace();
    }
}

QString RulesParser::getSelector()
{
    QRegularExpression rule(R"((\.?[a-zA-Z\(\[\.][/a-zA-Z0-9\+\-\[\]\(\)\_\|\\\*\.\^$\?:]*))");
    QRegularExpressionMatch ruleMath = rule.match(_data.mid(_id));
    if (ruleMath.hasMatch() && ruleMath.capturedStart() != 0)
    {
        return QString();
    }
    _id += ruleMath.capturedEnd();
    return ruleMath.captured(1);
}

/*QStringList RulesParser::getSelectors()
{
    QStringList selectors;
    QString selector = getSelector();
    while (!selector.isEmpty())
    {
        lineSelector = _line;
        skipSpaceAndComments();
        selectors.append(selector);
        if (_data[_id] == '{') // start of rule
        {
            _id ++;
            return selectors; // error
        }
        qDebug()<<"> "<<selector<<lineSelector;
        _id ++;
        skipSpaceAndComments();
    }
    return selectors;
}*/

QString RulesParser::getPropertyName()
{
    QRegularExpression rule("([a-zA-Z][a-zA-Z0-9\\_\\-]*)[\t ]*:[\t ]*", QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch ruleMath = rule.match(_data.mid(_id));
    if (ruleMath.hasMatch() && ruleMath.capturedStart() != 0)
    {
        return QString();
    }
    _id += ruleMath.capturedEnd();
    return ruleMath.captured(1);
}

QString RulesParser::getPropertyValue()
{
    QRegularExpression rule(R"lit("?([a-zA-Z0-9/^\?\$\|:\_\-\+\\\[\]\(\)\.\* ]*)"?;?)lit",
                            QRegularExpression::MultilineOption | QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatch ruleMath = rule.match(_data.mid(_id));
    if (ruleMath.hasMatch() && ruleMath.capturedStart() != 0)
    {
        return QString();
    }
    _id += ruleMath.capturedEnd();

    return ruleMath.captured(1);
}

int RulesParser::errorLine() const
{
    return _errorLine;
}
