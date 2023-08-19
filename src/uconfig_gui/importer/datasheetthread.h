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

#ifndef DATASHEETTHREAD_H
#define DATASHEETTHREAD_H

#include <QThread>

#include <pdf_extract/datasheet.h>

class DataSheetThread : public QThread
{
    Q_OBJECT
public:
    explicit DataSheetThread(Datasheet *datasheet);
    ~DataSheetThread() override;

    bool open(const QString &fileName);
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

#endif  // DATASHEETTHREAD_H
