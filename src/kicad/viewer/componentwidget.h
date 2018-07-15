#ifndef COMPONENTWIDGET_H
#define COMPONENTWIDGET_H

#include "kicad_global.h"

#include <QWidget>
#include <QToolButton>
#include <QComboBox>

#include "componentviewer.h"

class KICAD_EXPORT ComponentWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComponentWidget(QWidget *parent = Q_NULLPTR);

    Component *component() const;
    void setComponent(Component *component);

    ComponentViewer *viewer() const;

signals:

public slots:
    void setUnit(int unit);

protected:
    void createWidgets();

    Component *_component;
    ComponentViewer *_viewer;
    QToolButton *_toolGrid;
    QToolButton *_toolElecType;
    QComboBox *_comboUnit;
};

#endif // COMPONENTWIDGET_H
