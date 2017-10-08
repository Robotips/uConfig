#ifndef MODULEVIEWER_H
#define MODULEVIEWER_H

#include "kicad_global.h"

#include <QGraphicsView>
#include <QList>

#include "component.h"

class ComponentItem;
class PinItem;

class KICAD_EXPORT ComponentViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ComponentViewer(QWidget *parent = Q_NULLPTR);

    Component *component() const;
    void setComponent(Component *component);

public slots:
    void selectPin(Pin *pin);
    void selectPins(QList<Pin *>pins);

signals:
    void pinSelected(Pin *pin);

protected:
    void wheelEvent(QWheelEvent *event);

protected slots:
    void selectedItem();

protected:
    Component *_component;
    ComponentItem *_componentItem;
    double _currentZoomLevel;
};

#endif // MODULEVIEWER_H
