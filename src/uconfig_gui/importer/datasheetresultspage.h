#ifndef DATASHEETRESULTSPAGE_H
#define DATASHEETRESULTSPAGE_H

#include <QWizardPage>
#include <QLabel>

#include "../pdf_extract/datasheet.h"

class DatasheetResultsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DatasheetResultsPage();

    bool isComplete() const;

    int nextId() const;

    void initializePage();

    Datasheet *datasheet();

private:
    Datasheet _datasheet;
    QLabel *_resultLabel;
};

#endif // DATASHEETRESULTSPAGE_H
