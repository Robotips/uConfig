#ifndef DATASHEETTHREAD_H
#define DATASHEETTHREAD_H

#include <QThread>

#include "../pdf_extract/datasheet.h"

class DataSheetThread : public QThread
{
    Q_OBJECT
public:
    explicit DataSheetThread(Datasheet *datasheet);

signals:

    // QThread interface
protected:
    Datasheet *_datasheet;
    virtual void run();
};

#endif // DATASHEETTHREAD_H
