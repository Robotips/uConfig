#include "rule.h"

#include <QDebug>

Rule::Rule(const QString &selector)
    : _selector(selector, QRegularExpression::CaseInsensitiveOption)
{
    _enabled = true;
}

Rule::~Rule()
{
}

QRegularExpression Rule::selector() const
{
    return _selector;
}

void Rule::setSelector(const QRegularExpression &selector)
{
    _selector = selector;
}

void Rule::setSelector(const QString &selector)
{
    _selector.setPattern(selector);
    _selector.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
}

bool Rule::matchWithName(const QString &name) const
{
    if (!_enabled)
        return false;
    if (!_selector.isValid())
        return false;

    QRegularExpressionMatch match = _selector.match(name);
    return match.hasMatch();
}

bool Rule::isEnabled() const
{
    return _enabled;
}

void Rule::setEnabled(bool enabled)
{
    _enabled = enabled;
}

QString Rule::file() const
{
    return _file;
}

void Rule::setFile(const QString &file)
{
    _file = file;
}

int Rule::line() const
{
    return _line;
}

void Rule::setLine(int line)
{
    _line = line;
}
