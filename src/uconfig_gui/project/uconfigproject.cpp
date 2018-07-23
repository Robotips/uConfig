#include "uconfigproject.h"

#include <QFileDialog>
#include <QMessageBox>

#include "importer/pinlistimporter.h"

UConfigProject::UConfigProject(QWidget *window)
{
    setWindow(window);
    _lib = Q_NULLPTR;
    _activeComponent = Q_NULLPTR;
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
    selectComponent(Q_NULLPTR);
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
        if (!_libFileName.isEmpty())
            fileDialog.setDirectory(QFileInfo(_libFileName).dir());
        if (fileDialog.exec())
            mlibFileName = fileDialog.selectedFiles().first();
        if (mlibFileName.isEmpty())
            return;
    }

    _importedPathLib = mlibFileName;
    _libFileName = mlibFileName;
    _lib = new Lib();
    _lib->readFrom(mlibFileName);
    _libName = _lib->name();
    emit libChanged(_lib);
    if (!_lib->components().empty())
        selectComponent(_lib->components()[0]);
    else
        selectComponent(Q_NULLPTR);
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
            libFileName.replace(QRegExp("(.*)\\.(pdf|csv)"), "\\1.lib");
            fileDialog.selectFile(libFileName);
        }
        if (fileDialog.exec())
            libFileName = fileDialog.selectedFiles().first();
        if (libFileName.isEmpty())
            return;
    }
    else
        libFileName = fileName;

    if (!libFileName.endsWith(".lib"))
        libFileName.append(".lib");
    _libFileName = libFileName;

    _libName = _lib->name();
    emit libChanged(_lib);

    _lib->saveTo(libFileName);
}

void UConfigProject::importComponents(const QString &fileName)
{
    if (!_lib)
        _lib = new Lib();

    PinListImporter importer(fileName, _window);
    if (importer.exec() != QDialog::Accepted)
        return;

    foreach (Component *component, importer.components())
        _lib->addComponent(component);

    _importedPathLib = importer.filePath();
    emit libChanged(_lib);

    if (!_lib->components().empty())
        selectComponent(_lib->components()[0]);
}

void UConfigProject::closeLib()
{
    if (!_lib)
        return;
    if (_libName == tr("newlib") && _lib->componentsCount() == 0)
        return;
    if (QMessageBox::question(_window, tr("Saves lib?"), tr("Do you want to save '%1' library? Modifications will be losted.")
                             .arg(_lib->name()),
                              QMessageBox::Yes | QMessageBox::Default, QMessageBox::No) == QMessageBox::Yes)
        saveLib();
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
    if (!_activeComponent)
        return;

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
