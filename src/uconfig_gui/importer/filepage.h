#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QSettings>
#include <QWizardPage>

#include "pinlistimporter.h"

class FilePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit FilePage();

    bool isComplete() const override;
    int nextId() const override;
    void initializePage() override;

    Q_PROPERTY(QString file READ file)
    QString file() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void fileExplore();
    void checkEntry(const QString &text);

private:
    bool _complete;
    QString _file;
    QString _fileTitle;
    QLineEdit *_fileEdit;
    QStringList _suffixes;
    QSettings _settings;
    void setFile(const QString &file);
    static QMap<PinListImporter::ImportType, QString> _importTypeSettingsKeymap;
};

#endif // FILEPAGE_H
