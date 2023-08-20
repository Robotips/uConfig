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

#ifndef COMPONENTPINSTABLEVIEW_H
#define COMPONENTPINSTABLEVIEW_H

#include <QAction>
#include <QTableView>

#include "componentpindelegate.h"
#include "componentpinsitemmodel.h"
#include "componentpinssortfilterproxymodel.h"

class KICAD_EXPORT ComponentPinsTableView : public QTableView
{
    Q_OBJECT
public:
    ComponentPinsTableView(Component *component = nullptr, QWidget *parent = nullptr);

    Component *component() const;
    void setComponent(Component *component);

    ComponentPinsItemModel *model() const;

    QAction *removeAction() const;

public slots:
    void selectPin(Pin *pin);
    void selectPins(QList<Pin *> pins);
    void setPinFilter(const QString &filter);
    void remove();
    void copy();

signals:
    void pinSelected(QList<Pin *> pins);

protected slots:
    void updateSelect(const QItemSelection &selected, const QItemSelection &deselected);

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

protected:
    ComponentPinsItemModel *_model;
    ComponentPinDelegate *_delegate;
    ComponentPinsSortFilterProxyModel *_sortProxy;

    void createActions();
    QAction *_removeAction;
    QAction *_copyAction;
};

#endif  // COMPONENTPINSTABLEVIEW_H
