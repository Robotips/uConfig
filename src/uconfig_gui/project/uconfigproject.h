#ifndef UCONFIGPROJECT_H
#define UCONFIGPROJECT_H

#include <QObject>

#include "model/lib.h"

class UConfigProject : public QObject
{
    Q_OBJECT
public:
    explicit UConfigProject(QWidget *window=Q_NULLPTR);
    ~UConfigProject();

    Lib *lib() const;
    const QString &libFileName() const;
    const QString &libName() const;

    QWidget *window() const;
    void setWindow(QWidget *window);

    enum ComponentInfoType {
        ComponentNameInfo,
        ComponentPackagesInfo,
        ComponentReferenceInfo,
        ComponentAliasesInfo
    };

    Component *activeComponent() const;

    QList<QString> oldProjects() const;
    static const int MaxOldProject;

public slots:
    void newLib();
    void openLib(const QString &libFileName=QString());
    void saveLib();
    void saveLibAs(const QString &fileName=QString());
    void importComponents(const QString &fileName=QString());
    bool closeLib();

    void selectComponent(Component *component);

    void setComponentInfo(ComponentInfoType infoType, const QVariant &value);

signals:
    void libChanged(Lib *lib);
    void activeComponentChange(Component *component);
    void componentChange(Component *component);
    void pinChanged();
    void oldProjectChanged();

protected:
    Lib *_lib;
    QString _libFileName;
    QString _libName;
    QString _importedPathLib;

    Component *_activeComponent;

    QWidget *_window;

    void writeSettings();
    void readSettings();
    QList<QString> _oldProjects;
};

#endif // UCONFIGPROJECT_H
