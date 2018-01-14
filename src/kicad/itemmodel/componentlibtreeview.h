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
    void setLib(Lib *lib);

    void addComponent(Component *component);
    QList<Component *> components() const;

    bool selectedMode() const;
    void setSelectedMode(bool selectedMode);
    const QList<Component *> &selectedComponents() const;

    bool editMode() const;
    void setEditMode(bool editMode);

public slots:
    void selectAll(bool selected=true);
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
