#include "uconfigproject.h"

#include <QFileDialog>

#include "importer/pinlistimporter.h"

UConfigProject::UConfigProject(QWidget *window)
{
    setWindow(window);
    _activeComponent = Q_NULLPTR;
}

Lib *UConfigProject::lib() const
{
    return _lib;
}

QString UConfigProject::libFileName() const
{
    return _libFileName;
}

void UConfigProject::newLib()
{
    _lib = new Lib();
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
        fileDialog.setWindowTitle(tr("Save Kicad library"));
        if (fileDialog.exec())
            mlibFileName = fileDialog.selectedFiles().first();
        if (mlibFileName.isEmpty())
            return;
    }

    _importedPathLib = mlibFileName;
    _lib = new Lib();
    _lib->readFrom(mlibFileName);
    if (!_lib->components().empty())
        selectComponent(_lib->components()[0]);
    else
        selectComponent(Q_NULLPTR);
    emit libChanged(_lib);
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
        fileDialog.setWindowTitle(tr("Save Kicad library"));
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

    _lib->saveTo(libFileName);
}

void UConfigProject::importComponents(const QString &fileName)
{
    PinListImporter importer(fileName, _window);
    if (importer.exec() != QDialog::Accepted)
        return;

    foreach (Component *component, importer.components())
        _lib->addComponent(component);

    if (!_lib->components().empty())
        selectComponent(_lib->components()[0]);

    _importedPathLib = importer.filePath();
    emit libChanged(_lib);
}

void UConfigProject::selectComponent(Component *component)
{
    if (component != _activeComponent)
    {
        _activeComponent = component;
        emit activeComponentChange(component);
    }

    if (!component)
        return;

    /*organize(_ruleComboBox->currentText());
    if (!component->debugInfo().isNull())
    {
        QImage image = component->debugInfo();
        image = image.scaledToHeight(QApplication::primaryScreen()->size().height());
        _pdfDebug->setPixmap(QPixmap::fromImage(image));
    }*/
}

QWidget *UConfigProject::window() const
{
    return _window;
}

void UConfigProject::setWindow(QWidget *window)
{
    _window = window;
}

