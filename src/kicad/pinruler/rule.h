#ifndef RULE_H
#define RULE_H

#include "kicad_global.h"

#include <QString>
#include <QRegularExpression>

class KICAD_EXPORT Rule
{
public:
    Rule(const QString &selector = QString(".*"));
    virtual ~Rule();

    enum Type
    {
        PinRuleType,
        ClassRuleType
    };
    virtual Type type() const =0;

    QRegularExpression selector() const;
    void setSelector(const QRegularExpression &selector);
    void setSelector(const QString &selector);
    bool matchWithName(const QString &name) const;

    bool isEnabled() const;
    void setEnabled(bool enabled);

    virtual bool setProperty(const QString &name, const QString &value) =0;
    virtual bool hasPropertySet(const QString &name) const =0;

    // debug
    QString file() const;
    void setFile(const QString &file);

    int line() const;
    void setLine(int line);

protected:
    QRegularExpression _selector;
    bool _enabled;

    // debug purpose
    QString _file;
    int _line;
};

#endif  // RULE_H
