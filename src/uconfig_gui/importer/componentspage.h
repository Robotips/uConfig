#ifndef COMPONENTSPAGE_H
#define COMPONENTSPAGE_H

#include <QCheckBox>
#include <QLabel>
#include <QWizardPage>

#include "../kicad/itemmodel/componentlibtreeview.h"

class ComponentsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ComponentsPage();

    bool isComplete() const override;
    int nextId() const override;
    void initializePage() override;

protected:
    void finish();
    void changePage(int page);
    void addLog(QString str);
    bool validatePage() override;

private:
    QCheckBox *_checkAllBox;
    QLabel *_statusLabel;
    ComponentLibTreeView *_componentTreeView;
    Lib *_lib;
};

#endif // COMPONENTSPAGE_H
