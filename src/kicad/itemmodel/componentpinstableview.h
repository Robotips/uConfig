#ifndef COMPONENTPINSTABLEVIEW_H
#define COMPONENTPINSTABLEVIEW_H

#include "kicad_global.h"

#include <QTableView>

#include "componentpinsitemmodel.h"

class KICAD_EXPORT ComponentPinsTableView : public QTableView
{
    Q_OBJECT
public:
    ComponentPinsTableView(Component *component, QWidget *parent = Q_NULLPTR);

    Component *component() const;
    void setComponent(Component *component);

protected:
    ComponentPinsItemModel *_model;
};

#endif // COMPONENTPINSTABLEVIEW_H
