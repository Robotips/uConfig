#ifndef COMPONENTLIBTREEVIEW_H
#define COMPONENTLIBTREEVIEW_H

#include "kicad_global.h"

#include <QTreeView>
#include <QSortFilterProxyModel>

#include "componentlibitemmodel.h"

class KICAD_EXPORT ComponentLibTreeView : public QTreeView
{
    Q_OBJECT
public:
    ComponentLibTreeView(Lib *lib = Q_NULLPTR, QWidget *parent = Q_NULLPTR);

    Lib *lib() const;
    void setLib(Lib *lib);

    void addComponent(Component *component);
    QList<Component *> components() const;

signals:
    void openedComponent(Component * component);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

protected:
    ComponentLibItemModel *_model;
    QSortFilterProxyModel *_sortProxy;
};

#endif // COMPONENTLIBTREEVIEW_H
