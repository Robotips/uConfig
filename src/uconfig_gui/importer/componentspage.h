#ifndef COMPONENTSPAGE_H
#define COMPONENTSPAGE_H

#include <QWizardPage>
#include <QLabel>

#include "../kicad/itemmodel/componentlibtreeview.h"

class ComponentsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ComponentsPage();

    bool isComplete() const;
    int nextId() const;
    void initializePage();

protected:
    void finish();
    void changePage(int page);
    void addLog(QString str);
    bool validatePage();

private:
    QLabel *_statusLabel;
    ComponentLibTreeView *_componentTreeView;
    Lib *_lib;
};

#endif // COMPONENTSPAGE_H
