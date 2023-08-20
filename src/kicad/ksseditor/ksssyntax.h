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

#ifndef KSSSYNTAX_H
#define KSSSYNTAX_H

#include <QRegularExpression>
#include <QSyntaxHighlighter>

class KICAD_EXPORT KSSSyntax : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    KSSSyntax(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

protected:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    struct PartToHighlight
    {
        HighlightingRule rule;
        int index;
        int length;
    };
    friend bool lessThan(const PartToHighlight &t1, const PartToHighlight &t2);

    QTextCharFormat classFormat;
    QTextCharFormat keywordFormat;
    QTextCharFormat enumvaluesFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat numberFormat;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
};

inline bool lessThan(const KSSSyntax::PartToHighlight &t1, const KSSSyntax::PartToHighlight &t2)
{
    return t1.index < t2.index;
}

#endif  // KSSSYNTAX_H
