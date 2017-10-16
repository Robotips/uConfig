#ifndef COMPONENTPINSTABLEVIEW_H
#define COMPONENTPINSTABLEVIEW_H

#include "kicad_global.h"

#include <QTableView>

#include "numericalsortfilterproxymodel.h"
#include "componentpinsitemmodel.h"

class KICAD_EXPORT ComponentPinsTableView : public QTableView
{
    Q_OBJECT
public:
    ComponentPinsTableView(Component *component = Q_NULLPTR, QWidget *parent = Q_NULLPTR);

    Component *component() const;
    void setComponent(Component *component);

public slots:
    void selectPin(Pin *pin);

signals:
    void pinSelected(Pin *pin);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event);

protected:
    ComponentPinsItemModel *_model;
    NumericalSortFilterProxyModel *_sortProxy;
};

#endif // COMPONENTPINSTABLEVIEW_H
