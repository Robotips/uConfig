/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
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
    foreach (const QString &pattern, keywordPatterns)
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

    foreach (const QString &pattern, enumvaluesPatterns)
    {
        rule.pattern.setPattern("\\b(" + pattern + ")\\b");
        rule.format = enumvaluesFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(QColor(128, 0, 128));
    rule.pattern.setPattern("(\"[^\"]*\")");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    numberFormat.setForeground(QColor(255, 69, 0));
    rule.pattern.setPattern("\\b([0-9]+)\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(0, 128, 0));
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void KSSSyntax::highlightBlock(const QString &text)
{
    QList<PartToHighlight> partsToHighlight;
    PartToHighlight highlight;

    partsToHighlight.clear();
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        if (index >= 0)
        {
            highlight.index = expression.pos(1);
            highlight.length = expression.cap(1).length();
            highlight.rule = rule;
            partsToHighlight.append(highlight);
        }
    }

    int index = 0;
    qSort(partsToHighlight);
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
        QRegExp expression(partToHighlight.rule.pattern);
        int find = expression.indexIn(text, index);
        if (find >= 0)
        {
            highlight.index = expression.pos(1);
            highlight.length = expression.cap(1).length();
            highlight.rule = partToHighlight.rule;
            partsToHighlight.append(highlight);
            qSort(partsToHighlight);
        }
    }
    setCurrentBlockState(0);

    if (!commentStartExpression.isEmpty() && !commentEndExpression.isEmpty())
    {
        int startIndex = 0;
        if (previousBlockState() != 1)
        {
            startIndex = commentStartExpression.indexIn(text);
        }

        while (startIndex >= 0)
        {
            int endIndex = commentEndExpression.indexIn(text, startIndex);
            int commentLength;
            if (endIndex == -1)
            {
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }
            else
            {
                commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
            }
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }
}
