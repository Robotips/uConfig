#ifndef PDFFILEPAGE_H
#define PDFFILEPAGE_H

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QWizardPage>

#include "datasheetthread.h"

#include "pinlistimporter.h"

class PDFFilePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit PDFFilePage(DataSheetThread *datasheetThread);

    bool isComplete() const override;
    int nextId() const override;
    void initializePage() override;

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
