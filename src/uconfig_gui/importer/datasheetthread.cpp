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

#include "datasheetthread.h"

#include <QDebug>

DataSheetThread::DataSheetThread(Datasheet *datasheet)
{
    _datasheet = datasheet;
    _datasheet->moveToThread(this);
    _pageBegin = -1;
    _pageEnd = -1;
}

DataSheetThread::~DataSheetThread()
{
    if (isRunning())
    {
        quit();
        terminate();
    }
    delete _datasheet;
}

bool DataSheetThread::open(const QString &fileName)
{
    return _datasheet->open(fileName);
}

void DataSheetThread::close()
{
    _datasheet->close();
}

void DataSheetThread::setRange(int pageBegin, int pageEnd)
{
    _pageBegin = pageBegin;
    _pageEnd = pageEnd;
}

void DataSheetThread::setForceEnabled(bool force)
{
    _datasheet->setForceEnabled(force);
}

Datasheet *DataSheetThread::datasheet() const
{
    return _datasheet;
}

int DataSheetThread::pageBegin() const
{
    return _pageBegin;
}

int DataSheetThread::pageEnd() const
{
    return _pageEnd;
}

void DataSheetThread::run()
{
    setTerminationEnabled();
    _datasheet->analyse(_pageBegin, _pageEnd);
}
