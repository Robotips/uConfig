#ifndef DATASHEETTHREAD_H
#define DATASHEETTHREAD_H

#include <QThread>

#include "../pdf_extract/datasheet.h"

class DataSheetThread : public QThread
{
    Q_OBJECT
public:
    explicit DataSheetThread(Datasheet *datasheet);
    ~DataSheetThread() override;

    bool open(QString fileName);
    void close();

    void setRange(int pageBegin = -1, int pageEnd = -1);
    int pageBegin() const;
    int pageEnd() const;
    void setForceEnabled(bool force);

    Datasheet *datasheet() const;
signals:

    // QThread interface
protected:
    Datasheet *_datasheet;
    int _pageBegin;
    int _pageEnd;
    void run() override;
};

#endif // DATASHEETTHREAD_H
