#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QWizardPage>
#include <QLineEdit>
#include <QLabel>

#include "../pdf_extract/datasheet.h"

#include "pinlistimporter.h"

class FilePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit FilePage();

    bool isComplete() const;
    int nextId() const;
    void initializePage();

    Q_PROPERTY(QString file READ file)
    QString file() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void fileExplore();
    void checkEntry(const QString &text);

private:
    bool _complete;
    QString _file;
    QString _fileTitle;
    QLineEdit *_fileEdit;
    QLabel *_pagePreviewLabel;
    QStringList _suffixes;

    void setFile(const QString &file);
};

#endif // FILEPAGE_H
