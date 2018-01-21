#ifndef COMPONENTINFOSEDITOR_H
#define COMPONENTINFOSEDITOR_H

#include "kicad_global.h"

#include <QWidget>

#include <QLineEdit>
#include <QComboBox>

#include "component.h"

class KICAD_EXPORT ComponentInfosEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ComponentInfosEditor(QWidget *parent = Q_NULLPTR);

    Component *component() const;

signals:

public slots:
    void setComponent(Component *component);

protected:
    Component *_component;

    void createWidgets();
    QLineEdit *_nameEdit;
    QComboBox *_packageCombo;
};

#endif // COMPONENTINFOSEDITOR_H
