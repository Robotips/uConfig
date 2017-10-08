#ifndef COMPONENTLIBTREEVIEW_H
#define COMPONENTLIBTREEVIEW_H

#include "kicad_global.h"

#include <QTreeView>

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

protected:
    ComponentLibItemModel *_model;
};

#endif // COMPONENTLIBTREEVIEW_H
