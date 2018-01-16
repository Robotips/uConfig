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

    ComponentPinsItemModel *model() const;

public slots:
    void selectPin(Pin *pin);
    void selectPins(QList<Pin *> pins);

signals:
    void pinSelected(QList<Pin *> pins);

protected slots:
    void updateSelect(const QItemSelection &selected, const QItemSelection &deselected);

protected:
    ComponentPinsItemModel *_model;
    NumericalSortFilterProxyModel *_sortProxy;
};

#endif // COMPONENTPINSTABLEVIEW_H
