#ifndef RESULTSPAGE_H
#define RESULTSPAGE_H

#include <QWizardPage>
#include <QLabel>

#include "datasheetthread.h"

class ResultsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ResultsPage();

    bool isComplete() const;
    int nextId() const;
    void initializePage();

private:
    QLabel *_resultLabel;
};

#endif // RESULTSPAGE_H
