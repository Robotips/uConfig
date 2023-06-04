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

#include "uconfigproject.h"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSettings>

#include "importer/pinlistimporter.h"

const int UConfigProject::MaxOldProject = 8;

UConfigProject::UConfigProject(QWidget *window)
{
    setWindow(window);
    readSettings();
    _lib = nullptr;
    _activeComponent = nullptr;
}

UConfigProject::~UConfigProject()
{
    writeSettings();
}

Lib *UConfigProject::lib() const
{
    return _lib;
}

const QString &UConfigProject::libFileName() const
{
    return _libFileName;
}

const QString &UConfigProject::libName() const
{
    return _libName;
}

void UConfigProject::newLib()
{
    _lib = new Lib();
    _libName = tr("newlib");
    _libFileName = QString();
    selectComponent(nullptr);
    emit libChanged(_lib);
}

void UConfigProject::openLib(const QString &libFileName)
{
    QString mlibFileName = libFileName;

    if (mlibFileName.isEmpty())
    {
        QFileDialog fileDialog(_window);
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.setDefaultSuffix(".lib");
        fileDialog.setNameFilter(tr("Kicad component library (*.lib)"));
        fileDialog.setWindowTitle(tr("Open Kicad library"));
        fileDialog.setOption(QFileDialog::DontUseNativeDialog);
        QList<QUrl> urls;
        urls << QUrl::fromLocalFile("/usr/share/kicad");
        fileDialog.setSidebarUrls(urls);
        if (!_libFileName.isEmpty())
        {
            fileDialog.setDirectory(QFileInfo(_libFileName).dir());
        }
        if (fileDialog.exec() != 0)
        {
            mlibFileName = fileDialog.selectedFiles().first();
        }
        if (mlibFileName.isEmpty())
        {
            return;
        }
    }

    _importedPathLib = mlibFileName;
    _libFileName = mlibFileName;
    _lib = new Lib();
    _lib->readFrom(mlibFileName);
    _libName = _lib->name();
    emit libChanged(_lib);
    if (!_lib->components().empty())
    {
        selectComponent(_lib->components()[0]);
    }
    else
    {
        selectComponent(nullptr);
    }

    _oldProjects.removeOne(mlibFileName);
    _oldProjects.prepend(mlibFileName);
    if (_oldProjects.size() > MaxOldProject)
    {
        _oldProjects.removeLast();
    }
    emit oldProjectChanged();
}

void UConfigProject::saveLib()
{
    saveLibAs(_libFileName);
}

void UConfigProject::saveLibAs(const QString &fileName)
{
    QString libFileName;

    if (fileName.isEmpty())
    {
        QFileDialog fileDialog(_window);
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        fileDialog.setDefaultSuffix(".lib");
        fileDialog.setNameFilter(tr("Kicad component library (*.lib)"));
        fileDialog.setWindowTitle(tr("Save Kicad library as..."));
        if (!_importedPathLib.isEmpty())
        {
            libFileName = _importedPathLib;
            libFileName.replace(QRegularExpression("(.*)\\.(pdf|csv)"), "\\1.lib");
            fileDialog.selectFile(libFileName);
        }
        if (fileDialog.exec() != 0)
        {
            libFileName = fileDialog.selectedFiles().first();
        }
        if (libFileName.isEmpty())
        {
            return;
        }
    }
    else
    {
        libFileName = fileName;
    }

    if (!libFileName.endsWith(".lib", Qt::CaseInsensitive))
    {
        libFileName.append(".lib");
    }
    _libFileName = libFileName;

    _libName = _lib->name();
    emit libChanged(_lib);

    _oldProjects.removeOne(_libFileName);
    _oldProjects.prepend(_libFileName);
    if (_oldProjects.size() > MaxOldProject)
    {
        _oldProjects.removeLast();
    }
    emit oldProjectChanged();
    _lib->saveTo(libFileName);
}

void UConfigProject::importComponents(const QString &fileName)
{
    if (_lib == nullptr)
    {
        _lib = new Lib();
    }

    PinListImporter importer(fileName, _window);
    if (importer.exec() != QDialog::Accepted)
    {
        return;
    }

    for (Component *component : importer.components())
    {
        _lib->addComponent(component);
    }

    _importedPathLib = importer.filePath();
    emit libChanged(_lib);

    if (!_lib->components().empty())
    {
        selectComponent(_lib->components()[0]);
    }
}

bool UConfigProject::closeLib()
{
    if (_lib == nullptr)
    {
        return true;
    }
    if (_libName == tr("newlib") && _lib->componentsCount() == 0)
    {
        return true;
    }
    int ret = QMessageBox::question(_window,
                                    tr("Saves lib?"),
                                    tr("Do you want to save '%1' library? Modifications will be losted.").arg(_lib->name()),
                                    QMessageBox::Yes | QMessageBox::Default,
                                    QMessageBox::No,
                                    QMessageBox::Cancel);
    if (ret == QMessageBox::Yes)
    {
        saveLib();
    }
    return (ret != QMessageBox::Cancel);
}

void UConfigProject::selectComponent(Component *component)
{
    if (component != _activeComponent)
    {
        _activeComponent = component;
        emit activeComponentChange(component);
    }
}

void UConfigProject::setComponentInfo(UConfigProject::ComponentInfoType infoType, const QVariant &value)
{
    if (_activeComponent == nullptr)
    {
        return;
    }

    switch (infoType)
    {
        case UConfigProject::ComponentNameInfo:
            _activeComponent->setName(value.toString());
            break;
        case UConfigProject::ComponentPackagesInfo:
            _activeComponent->footPrints() = value.toStringList();
            break;
        case UConfigProject::ComponentReferenceInfo:
            _activeComponent->setPrefix(value.toString());
            break;
        case UConfigProject::ComponentAliasesInfo:
            _activeComponent->aliases() = value.toStringList();
            break;
    }

    emit componentChange(_activeComponent);
}

void UConfigProject::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    // old projects write
    settings.beginWriteArray("projects");
    for (int i = 0; i < _oldProjects.size() && i < MaxOldProject; ++i)
    {
        settings.setArrayIndex(i);
        QString path = _oldProjects[i];
        settings.setValue("path", path);
    }
    settings.endArray();
}

void UConfigProject::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    // old projects read
    int size = settings.beginReadArray("projects");
    for (int i = 0; i < size && i < MaxOldProject; ++i)
    {
        settings.setArrayIndex(i);
        QString path = settings.value("path", "").toString();
        if (!_oldProjects.contains(path) && !path.isEmpty())
        {
            _oldProjects.append(path);
        }
    }
    settings.endArray();
}

QList<QString> UConfigProject::oldProjects() const
{
    return _oldProjects;
}

Component *UConfigProject::activeComponent() const
{
    return _activeComponent;
}

QWidget *UConfigProject::window() const
{
    return _window;
}

void UConfigProject::setWindow(QWidget *window)
{
    _window = window;
}
