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

#include "ksssyntax.h"

#include <QDebug>

#include <algorithm>

KSSSyntax::KSSSyntax(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    classFormat.setForeground(QColor(64, 0, 255));
    rule.pattern.setPattern("(^\\..*)\\{");
    rule.format = classFormat;
    highlightingRules.append(rule);

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "name"
                    << "position"
                    << "sort"
                    << "sortpattern"
                    << "class"
                    << "length"
                    << "elecType"
                    << "visibility"
                    << "label"
                    << "rect"
                    << "priority";
    for (const QString &pattern : keywordPatterns)
    {
        rule.pattern.setPattern("\\b(" + pattern + ")\\b");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    enumvaluesFormat.setForeground(Qt::blue);
    enumvaluesFormat.setFontWeight(QFont::Bold);
    QStringList enumvaluesPatterns;
    enumvaluesPatterns << "top"
                       << "bottom"
                       << "left"
                       << "right"
                       << "aside"
                       << "none"
                       << "asc"
                       << "desc"
                       << "visible"
                       << "hidden"
                       << "removed"
                       << "in"
                       << "out"
                       << "bidir"
                       << "tri"
                       << "passive"
                       << "unspecified"
                       << "pin"
                       << "pout"
                       << "opcol"
                       << "opem"
                       << "nc"
                       << "norm"
                       << "novisible"
                       << "inverted"
                       << "clock"
                       << "iclock"
                       << "lowin"
                       << "clocklow"
                       << "lowout"
                       << "faledge"
                       << "nologic";

    for (const QString &pattern : enumvaluesPatterns)
    {
        rule.pattern.setPattern("\\b(" + pattern + ")\\b");
        rule.format = enumvaluesFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(QColor(128, 0, 128));
    rule.pattern.setPattern(R"(("[^"]*"))");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    numberFormat.setForeground(QColor(255, 69, 0));
    rule.pattern.setPattern("\\b([0-9]+)\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(0, 128, 0));
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void KSSSyntax::highlightBlock(const QString &text)
{
    QList<PartToHighlight> partsToHighlight;
    PartToHighlight highlight;

    partsToHighlight.clear();
    for (const HighlightingRule &rule : highlightingRules)
    {
        QRegularExpressionMatch match = rule.pattern.match(text);
        if (match.hasMatch())
        {
            highlight.index = match.capturedStart(1);
            highlight.length = match.capturedEnd(1);
            highlight.rule = rule;
            partsToHighlight.append(highlight);
        }
    }

    int index = 0;
    std::sort(partsToHighlight.begin(), partsToHighlight.end(), lessThan);
    while (!partsToHighlight.isEmpty())
    {
        PartToHighlight partToHighlight = partsToHighlight.takeFirst();

        // highlight if needed
        if (partToHighlight.index >= index)
        {
            setFormat(partToHighlight.index, partToHighlight.length, partToHighlight.rule.format);
            index = partToHighlight.index + partToHighlight.length;
        }

        // update part
        QRegularExpressionMatch match = partToHighlight.rule.pattern.match(text, index);
        if (match.hasMatch())
        {
            highlight.index = match.capturedStart(1);
            highlight.length = match.capturedEnd(1);
            highlight.rule = partToHighlight.rule;
            partsToHighlight.append(highlight);
            std::sort(partsToHighlight.begin(), partsToHighlight.end(), lessThan);
        }
    }
    setCurrentBlockState(0);

    if (commentStartExpression.isValid() && commentEndExpression.isValid())
    {
        int startIndex = 0;
        if (previousBlockState() != 1)
        {
            startIndex = text.indexOf(commentStartExpression);
        }

        while (startIndex >= 0)
        {
            QRegularExpressionMatch endMatch = commentEndExpression.match(text, startIndex);
            int endIndex = text.indexOf(commentEndExpression, startIndex);
            int commentLength;
            if (endMatch.hasMatch())
            {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }
            else
            {
                commentLength = endIndex - startIndex + endMatch.capturedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
        }
    }
}
