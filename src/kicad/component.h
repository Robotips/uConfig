#ifndef COMPONENT_H
#define COMPONENT_H

#include "kicad_global.h"

#include <QList>
#include <QString>
#include <QRect>
#include <QStringList>
#include <QImage>
#include "pin.h"

/**
 * @brief The Component class contains all information about one schematic component.
 * It could also have alias name, stored in the same component.
 */
class KICAD_EXPORT Component
{
public:
    Component(const QString &name = QString());
    Component(const Component &other);
    ~Component();

    const QString &name() const;
    void setName(const QString &name);

    QList<Pin *> pins();
    const QList<Pin *> pins() const;
    void addPin(Pin *pin);
    void removePin(Pin *pin);

    const QString &prefix() const;
    void setPrefix(const QString &prefix);

    const QStringList &aliases() const;
    void addAlias(const QString &alias);

    QStringList &footPrints();
    const QStringList &footPrints() const;
    void addFootPrint(const QString &footprint);

    const QRect &rect() const;
    void setRect(const QRect &rect);

    bool showPinName() const;
    void setShowPinName(bool showPinName);

    bool showPadName() const;
    void setShowPadName(bool showPadName);

    void sort();
    void reorganizeToPackageStyle();

    const QImage &debugInfo() const;
    void setDebugInfo(const QImage &debugInfo);

    bool isValid() const;

    friend QTextStream &operator<<(QTextStream &stream, const Component &component);
    friend QTextStream &operator>>(QTextStream &stream, Component &component);

private:
    QList<Pin *> _pins;
    QString _name;
    QString _prefix;
    QStringList _alias;
    QStringList _footPrints;
    QRect _rect;
    bool _valid;
    bool _showPinName;
    bool _showPadName;
    QImage _debugInfo;
};

#endif  // COMPONENT_H
