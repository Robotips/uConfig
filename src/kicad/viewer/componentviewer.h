#ifndef MODULEVIEWER_H
#define MODULEVIEWER_H

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

signals:

public slots:

protected:
    Component *_component;
};

#endif // MODULEVIEWER_H
