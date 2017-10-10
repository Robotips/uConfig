#include "datasheetthread.h"

DataSheetThread::DataSheetThread(Datasheet *datasheet)
{
    _datasheet = datasheet;
}

void DataSheetThread::run()
{
    _datasheet->analyse();
}
