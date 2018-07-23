#ifndef COMPONENTINFOSEDITOR_H
#define COMPONENTINFOSEDITOR_H

#include <QWidget>

#include <QLineEdit>
#include <QPlainTextEdit>

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
    void setComponentPackages();
    void setComponentAliases();

protected:
    UConfigProject *_project;
    Component *_component;

    void createWidgets();
    QLineEdit *_nameEdit;
    QPlainTextEdit *_packageEdit;
    QLineEdit *_referenceEdit;
    QPlainTextEdit *_aliasesEdit;
};

#endif // COMPONENTINFOSEDITOR_H
