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

bool DataSheetThread::open(QString fileName)
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
