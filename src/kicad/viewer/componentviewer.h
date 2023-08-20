/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
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

#include <QGraphicsView>
#include <QList>
#include <QtCore/qglobal.h>

#include "componentscene.h"
#include "model/component.h"

class ComponentScene;
class ComponentItem;
class PinItem;

class KICAD_EXPORT ComponentViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ComponentViewer(QWidget *parent = nullptr);

    Component *component() const;
    void setComponent(Component *component, int layer = 1);

public slots:
    void selectPin(Pin *pin);
    void selectPins(const QList<Pin *> &pins);
    void updatePin(Pin *pin);
    void removePin(Pin *pin);

    void setGridVisible(bool visible);
    void setGridFront(bool front);
    void setElecTypeVisible(bool visible);

signals:
    void pinsSelected(QList<Pin *> pins);
    void droppedFile(QString fileName);

protected:
    void wheelEvent(QWheelEvent *event) override;

protected slots:
    void selectedItem();

protected:
    ComponentScene *_scene;
    double _currentZoomLevel;

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
};

#endif  // MODULEVIEWER_H
