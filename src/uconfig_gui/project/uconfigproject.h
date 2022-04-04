/**
 ** This file is part of the uConfig project.
 ** Copyright 2017-2020 Robotips, Sebastien CAUX (sebcaux)
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef UCONFIGPROJECT_H
#define UCONFIGPROJECT_H

#include <QObject>

#include "model/lib.h"

class UConfigProject : public QObject
{
    Q_OBJECT
public:
    explicit UConfigProject(QWidget *window = nullptr);
    ~UConfigProject() override;

    Lib *lib() const;
    const QString &libFileName() const;
    const QString &libName() const;

    QWidget *window() const;
    void setWindow(QWidget *window);

    enum ComponentInfoType
    {
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
    void openLib(const QString &libFileName = QString());
    void saveLib();
    void saveLibAs(const QString &fileName = QString());
    void importComponents(const QString &fileName = QString());
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

#endif  // UCONFIGPROJECT_H
