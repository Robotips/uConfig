#ifndef RESULTSPAGE_H
#define RESULTSPAGE_H

#include <QLabel>
#include <QWizardPage>

#include "datasheetthread.h"

class ResultsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ResultsPage();

    bool isComplete() const override;
    int nextId() const override;
    void initializePage() override;

private:
    QLabel *_resultLabel;
};

#endif // RESULTSPAGE_H
