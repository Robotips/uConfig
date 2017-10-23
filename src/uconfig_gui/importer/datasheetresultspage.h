#ifndef DATASHEETRESULTSPAGE_H
#define DATASHEETRESULTSPAGE_H

#include <QWizardPage>
#include <QLabel>

#include "datasheetthread.h"

class DatasheetResultsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DatasheetResultsPage(DataSheetThread *datasheetThread, QList<Component *> &components);

    bool isComplete() const;
    int nextId() const;
    void initializePage();

private:
    DataSheetThread *_datasheetThread;
    QLabel *_resultLabel;
    QList<Component *> &_components;

    // QWizardPage interface
public:
    virtual bool validatePage();
};

#endif // DATASHEETRESULTSPAGE_H
