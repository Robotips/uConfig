#ifndef PINLISTEDITOR_H
#define PINLISTEDITOR_H

#include "componentpinstableview.h"
#include "kicad_global.h"

#include <QWidget>

#include <QLineEdit>

class KICAD_EXPORT PinListEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PinListEditor(QWidget *parent = Q_NULLPTR);

    Component *component() const;

    ComponentPinsTableView *tableView() const;

signals:

public slots:
    void setComponent(Component *component);

protected:
    void createWidgets();
    ComponentPinsTableView *_componentPinsTableView;
    QLineEdit *_nameFilterEdit;
};

#endif // PINLISTEDITOR_H
