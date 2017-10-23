#ifndef PDFFILEPAGE_H
#define PDFFILEPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QLabel>
#include <QRadioButton>
#include <QCheckBox>

#include "datasheetthread.h"

#include "pinlistimporter.h"

class PDFFilePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit PDFFilePage(DataSheetThread *datasheetThread);

    bool isComplete() const;
    int nextId() const;
    void initializePage();

private slots:
    void check();

private:
    DataSheetThread *_datasheetThread;
    bool _complete;
    QLabel *_pagePreviewLabel;
    QRadioButton *_allRadio;
    QRadioButton *_partialRadio;
    QLineEdit *_rangeEdit;
    QCheckBox *_forceCheckBox;
    QLabel *_pageCountLabel;

    QStringList _suffixes;
};

#endif // PDFFILEPAGE_H
