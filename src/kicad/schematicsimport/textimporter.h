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

#ifndef TEXTIMPORTER_H
#define TEXTIMPORTER_H

#include <QtCore/qglobal.h>

#include "schematicsimporter.h"

class KICAD_EXPORT TextImporter : public SchematicsImporter
{
public:
    TextImporter();

    QChar columnSeparator() const;
    void setColumnSeparator(const QChar &columnSeparator);

    int pinColumn() const;
    void setPinColumn(int pinColumn);

    QList<int> pinNameColumns() const;
    void setPinNameColumns(const QList<int> &pinNameColumns);

    QChar pinNameColumnsSeparator() const;
    void setPinNameColumnsSeparator(const QChar &pinNameColumnsSeparator);

    int pinElectricalTypeColumn() const;
    void setPinElectricalTypeColumn(int pinColumn);

protected:
    QChar _columnSeparator;
    int _pinColumn;
    QList<int> _pinNameColumns;
    QChar _pinNameColumnsSeparator;
    int _pinElectricalTypeColumn;

    // SchematicsImporter interface
public:
    bool import(const QString &fileName) override;
};

#endif  // TEXTIMPORTER_H
