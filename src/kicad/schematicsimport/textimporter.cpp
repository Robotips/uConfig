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

#include "textimporter.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>

static const QMap<QString, Pin::ElectricalType> pinElectricalTypeMap = {
    {"Input", Pin::ElectricalType::Input},
    {"Output", Pin::ElectricalType::Output},
    {"Bidir", Pin::ElectricalType::Bidir},
    {"Tristate", Pin::ElectricalType::Tristate},
    {"Passive", Pin::ElectricalType::Passive},
    {"Unspecified", Pin::ElectricalType::Unspecified},
    {"PowerIn", Pin::ElectricalType::PowerIn},
    {"PowerOut", Pin::ElectricalType::PowerOut},
    {"OpenCollector", Pin::ElectricalType::OpenCollector},
    {"OpenEmitter", Pin::ElectricalType::OpenEmitter},
    {"NotConnected", Pin::ElectricalType::NotConnected}
};

TextImporter::TextImporter()
{
    _columnSeparator = '\t';
    _pinColumn = 0;
    _pinNameColumns = {1};
    _pinNameColumnsSeparator = '/';
    _pinElectricalTypeColumn = 2;
}

QChar TextImporter::columnSeparator() const
{
    return _columnSeparator;
}

void TextImporter::setColumnSeparator(const QChar &columnSeparator)
{
    _columnSeparator = columnSeparator;
}

QChar TextImporter::pinNameColumnsSeparator() const
{
    return _pinNameColumnsSeparator;
}

void TextImporter::setPinNameColumnsSeparator(const QChar &pinNameColumnsSeparator)
{
    _pinNameColumnsSeparator = pinNameColumnsSeparator;
}

QList<int> TextImporter::pinNameColumns() const
{
    return _pinNameColumns;
}

void TextImporter::setPinNameColumns(const QList<int> &pinNameColumns)
{
    _pinNameColumns = pinNameColumns;
}

int TextImporter::pinColumn() const
{
    return _pinColumn;
}

void TextImporter::setPinColumn(int pinColumn)
{
    _pinColumn = pinColumn;
}

int TextImporter::pinElectricalTypeColumn() const
{
    return _pinElectricalTypeColumn;
}

void TextImporter::setPinElectricalTypeColumn(int pinElectricalTypeColumn)
{
    _pinElectricalTypeColumn = pinElectricalTypeColumn;
}

bool TextImporter::import(const QString &fileName)
{
    qDeleteAll(_components);
    _components.clear();

    if (_pinNameColumns.isEmpty())
    {
        return false;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    int minColumnCount = qMax(*std::max_element(_pinNameColumns.begin(), _pinNameColumns.end()), _pinColumn) + 1;
    if (minColumnCount < 2)
    {
        file.close();
        return false;
    }

    QFileInfo fileInfo(file);
    Component *component = new Component(fileInfo.completeBaseName());

    while (!file.atEnd())
    {
        const QByteArray &line = file.readLine();
        QString lineString = QString::fromLatin1(line);
        lineString.remove(-1, 1);

        QStringList columns = lineString.split(_columnSeparator);
        if (columns.count() < minColumnCount)
        {
            continue;
        }

        QString pinNumber = columns[_pinColumn];

        QStringList pinNameColumns;
        for (int column : qAsConst(_pinNameColumns))
        {
            pinNameColumns.append(columns[column]);
        }
        QString pinName = pinNameColumns.join(_pinNameColumnsSeparator);

        Pin *pin = new Pin(pinName, pinNumber);

        if (_pinElectricalTypeColumn < columns.size())
        {
            QString pinElectricalType = columns[_pinElectricalTypeColumn];
            Pin::ElectricalType electricalType = Pin::ElectricalType::Unspecified;
            if (pinElectricalTypeMap.contains(pinElectricalType))
            {
                electricalType = pinElectricalTypeMap.value(pinElectricalType);
            }
            pin->setElectricalType(electricalType);
        }

        component->addPin(pin);
    }

    component->reorganizeToPackageStyle();
    _components.append(component);

    file.close();
    return true;
}
