#include "ksssyntax.h"

#include <QDebug>

KSSSyntax::KSSSyntax(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "name" << "position" << "sort" << "sortpattern" << "class";
    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern.setPattern("\\b("+pattern+")\\b");
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    quotationFormat.setForeground(QColor(128,0,128));
    rule.pattern.setPattern("(\"[^\"]*\")");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    typesFormat.setForeground(QColor(255,48,99));
    QStringList typesPatterns;
    typesPatterns << "a";

    foreach (const QString &pattern, typesPatterns)
    {
        rule.pattern.setPattern("\\b("+pattern+")\\b");
        rule.format = typesFormat;
        highlightingRules.append(rule);
    }

    numberFormat.setForeground(QColor(255,69,0));
    rule.pattern.setPattern("\\b([0-9]+)\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(0,128,0));
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

    int index=0;
    qSort(partsToHighlight);
    while (!partsToHighlight.isEmpty())
    {
        PartToHighlight partToHighlight = partsToHighlight.takeFirst();

        // highlight if needed
        if(partToHighlight.index>=index)
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
            startIndex = commentStartExpression.indexIn(text);

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
                commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }
}
