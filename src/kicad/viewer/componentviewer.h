/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef MODULEVIEWER_H
#define MODULEVIEWER_H

#include "kicad_global.h"

#include <QGraphicsView>
#include <QList>

#include "model/component.h"
#include "componentscene.h"

class ComponentScene;
class ComponentItem;
class PinItem;

class KICAD_EXPORT ComponentViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ComponentViewer(QWidget *parent = Q_NULLPTR);

    Component *component() const;
    void setComponent(Component *component, int layer=1);

public slots:
    void selectPin(Pin *pin);
    void selectPins(QList<Pin *>pins);
    void updatePin(Pin *pin);
    void removePin(Pin *pin);

    void setGridVisible(bool visible);
    void setElecTypeVisible(bool visible);

signals:
    void pinsSelected(QList<Pin *>pins);
    void droppedFile(QString fileName);

protected:
    void wheelEvent(QWheelEvent *event);

protected slots:
    void selectedItem();

protected:
    ComponentScene *_scene;
    double _currentZoomLevel;

    // QWidget interface
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);
};

#endif // MODULEVIEWER_H
