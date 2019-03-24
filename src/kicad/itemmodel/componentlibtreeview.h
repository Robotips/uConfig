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

#ifndef COMPONENTLIBTREEVIEW_H
#define COMPONENTLIBTREEVIEW_H

#include "kicad_global.h"

#include <QTreeView>

#include <QAction>

#include "numericalsortfilterproxymodel.h"
#include "componentlibitemmodel.h"

class KICAD_EXPORT ComponentLibTreeView : public QTreeView
{
    Q_OBJECT
public:
    ComponentLibTreeView(Lib *lib = Q_NULLPTR, QWidget *parent = Q_NULLPTR);

    Lib *lib() const;

    void addComponent(Component *component);
    QList<Component *> components() const;

    bool selectedMode() const;
    void setSelectedMode(bool selectedMode);
    const QList<Component *> &selectedComponents() const;

    bool editMode() const;
    void setEditMode(bool editMode);

    ComponentLibItemModel *model() const;

public slots:
    void setLib(Lib *lib);
    void setActiveComponent(Component * component);
    void selectAll(bool selected=true);
    void updateComponent(Component * component);
    void remove();

signals:
    void openedComponent(Component * component);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent *event);

protected:
    ComponentLibItemModel *_model;
    NumericalSortFilterProxyModel *_sortProxy;
    bool _editMode;

    void createActions();
    QAction *_removeAction;
};

#endif // COMPONENTLIBTREEVIEW_H
