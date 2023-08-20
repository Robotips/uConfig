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

#ifndef PINLISTIMPORTER_H
#define PINLISTIMPORTER_H

#include <QWizard>

#include <kicad/model/component.h>

class DatasheetProcessPage;

class PinListImporter : public QWizard
{
    Q_OBJECT
public:
    PinListImporter(const QString &fileName, QWidget *parent = nullptr);
    ~PinListImporter() override;

    /**
     * When extending this enum make sure to extend
     * FilePage::_importTypeSettingsKeymap in the filepage.cpp as well!
     */
    enum ImportType
    {
        CSV,
        PDF,
        // Table,
        Kicad
    };
    ImportType type();
    void setType(ImportType type);

    QString filePath() const;
    void setFilePath(const QString &filePath);

    enum Page
    {
        PageStart,
        PageFile,
        PagePDFFile,
        PagePDFProcess,
        PageCSVFile,
        PageKicadFile,
        PageComponents,
        PageResults
    };

    QList<Component *> &components();

private:
    ImportType _type;
    QString _filePath;
    QList<Component *> _components;
    DatasheetProcessPage *_datasheetProcess;
};

#endif  // PINLISTIMPORTER_H
