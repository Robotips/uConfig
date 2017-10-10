#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QWizardPage>
#include <QLineEdit>

class FilePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit FilePage(const int type);

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
    QLineEdit *fileEdit;
    int _type;
    QStringList _suffixes;

    void setFile(const QString &file);
};

#endif // FILEPAGE_H
