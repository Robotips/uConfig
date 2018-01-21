#ifndef COMPONENTINFOSEDITOR_H
#define COMPONENTINFOSEDITOR_H

#include <QWidget>

#include <QLineEdit>
#include <QComboBox>

#include "project/uconfigproject.h"

class ComponentInfosEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ComponentInfosEditor(UConfigProject *project);

    Component *component() const;

signals:

public slots:
    void setComponent(Component *component);

protected slots:
    void setComponentName();
    void setComponentReference();

protected:
    UConfigProject *_project;
    Component *_component;

    void createWidgets();
    QLineEdit *_nameEdit;
    QComboBox *_packageCombo;
    QLineEdit *_referenceEdit;
};

#endif // COMPONENTINFOSEDITOR_H
