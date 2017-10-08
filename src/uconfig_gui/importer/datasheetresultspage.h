#ifndef DATASHEETRESULTSPAGE_H
#define DATASHEETRESULTSPAGE_H

#include <QWizardPage>
#include <QLineEdit>

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

private slots:
    void fileExplore();
    void checkEntry(const QString &text);

private:
    bool _complete;
    QString _file;
    QString _fileTitle;
    QLineEdit *fileEdit;
    int _type;
    QStringList _suffixes;

    Datasheet _datasheet;

    void setFile(const QString &file);
};

#endif // DATASHEETRESULTSPAGE_H
