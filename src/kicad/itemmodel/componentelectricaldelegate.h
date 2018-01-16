#ifndef COMPONENTELECTRICALDELEGATE_H
#define COMPONENTELECTRICALDELEGATE_H

#include "kicad_global.h"

#include <QItemDelegate>

class KICAD_EXPORT ComponentElectricalDelegate : public QItemDelegate
{
public:
    ComponentElectricalDelegate(QObject *parent = Q_NULLPTR);

    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // COMPONENTELECTRICALDELEGATE_H
