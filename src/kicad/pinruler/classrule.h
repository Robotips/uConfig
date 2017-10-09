#ifndef CLASSRULE_H
#define CLASSRULE_H

#include "kicad_global.h"

#include <QString>
#include <QStringList>

#include "rule.h"

class KICAD_EXPORT ClassRule : public Rule
{
public:
    ClassRule(const QString &selector = QString(".*"));

    virtual Type type() const;

    virtual bool setProperty(const QString &name, const QString &value);

    static QStringList propsName;
};

#endif // CLASSRULE_H
