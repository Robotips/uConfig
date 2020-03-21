#ifndef DATASHEETPROCESSPAGE_H
#define DATASHEETPROCESSPAGE_H

#include <QLabel>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QWizardPage>

#include "datasheetthread.h"

class DatasheetProcessPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DatasheetProcessPage();

    bool isComplete() const override;
    int nextId() const override;
    void initializePage() override;

    DataSheetThread *datasheetThread();

protected:
    void finish();
    void changePage(int page);
    void addLog(QString str);

private:
    Datasheet *_datasheet;
    DataSheetThread *_thread;
    QLabel *_statusLabel;
    QLabel *_progressLabel;
    QProgressBar *_progressBar;
    QPlainTextEdit *_logger;
    int _pageStart;
    int _pageCount;
    bool _complete;
};

#endif // DATASHEETPROCESSPAGE_H
