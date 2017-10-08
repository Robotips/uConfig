#ifndef MODULEVIEWER_H
#define MODULEVIEWER_H

#include "kicad_global.h"

#include <QGraphicsView>
#include <QGraphicsScene>

#include "component.h"

class KICAD_EXPORT ComponentViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ComponentViewer(QWidget *parent = NULL);

    Component *component() const;
    void setComponent(Component *component);

protected:
    void wheelEvent(QWheelEvent *event);

protected:
    Component *_component;
    double _currentZoomLevel;
};

#endif // MODULEVIEWER_H
