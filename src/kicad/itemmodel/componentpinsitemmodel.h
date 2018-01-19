#ifndef COMPONENTPINSITEMMODEL_H
#define COMPONENTPINSITEMMODEL_H

#include "kicad_global.h"

#include <QAbstractItemModel>

#include "component.h"

class KICAD_EXPORT ComponentPinsItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    ComponentPinsItemModel(Component *component = Q_NULLPTR, QObject *parent = Q_NULLPTR);

    Component *component() const;
    void setComponent(Component *component);

    Pin *pin(const QModelIndex &index) const;
    QModelIndex index(Pin *pin) const;

    enum Column {
        PinNumber,
        PinName,
        PinElecType,
        PinStyle,
        ColumnCount
    };

    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;

signals:
    void pinModified(Pin *pin);

private:
    Component *_component;
};

#endif // COMPONENTPINSITEMMODEL_H
