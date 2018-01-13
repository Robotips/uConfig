#ifndef KSSSYNTAX_H
#define KSSSYNTAX_H

#include "kicad_global.h"

#include <QSyntaxHighlighter>

class KICAD_EXPORT KSSSyntax : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    KSSSyntax(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

protected:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    struct PartToHighlight
    {
        HighlightingRule rule;
        int index;
        int length;
    };
    friend bool operator<(const PartToHighlight &t1, const PartToHighlight &t2);

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat typesFormat;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
};

inline bool operator<(const KSSSyntax::PartToHighlight &t1, const KSSSyntax::PartToHighlight &t2)
{
    return t1.index<t2.index;
}

#endif // KSSSYNTAX_H
