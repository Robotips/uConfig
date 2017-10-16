#ifndef COMPONENT_H
#define COMPONENT_H

#include "kicad_global.h"

#include <QList>
#include <QString>
#include <QRect>
#include <QStringList>
#include <QImage>
#include "pin.h"

class KICAD_EXPORT Component
{
public:
    Component(const QString &name = QString());
    Component(const Component &other);
    ~Component();

    QString name() const;
    void setName(const QString &name);

    QList<Pin *> pins();
    const QList<Pin *> pins() const;
    void addPin(Pin *pin);

    QString prefixe() const;
    void setPrefixe(const QString &prefixe);

    const QStringList &alias() const;
    void addAlias(const QString &alias);

    const QStringList &footPrints() const;
    void addFootPrints(const QString &footprint);

    QRect rect() const;
    void setRect(const QRect &rect);

    void sort();
    void reorganizeToPackageStyle();

    QImage debugInfo() const;
    void setDebugInfo(const QImage &debugInfo);

    friend QTextStream &operator<<(QTextStream &stream,
                                   const Component &component);
    friend QTextStream &operator>>(QTextStream &stream, Component &component);

    bool isValid() const;

private:
    QList<Pin *> _pins;
    QString _name;
    QString _prefixe;
    QStringList _alias;
    QStringList _footPrints;
    QRect _rect;
    bool _valid;
    QImage _debugInfo;
};

#endif  // COMPONENT_H
