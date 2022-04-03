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

#ifndef DATASHEETPROCESSPAGE_H
#define DATASHEETPROCESSPAGE_H

#include <QLabel>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QWizardPage>

#include "datasheetthread.h"

class DatasheetProcessPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DatasheetProcessPage();

    bool isComplete() const override;
    int nextId() const override;
    void initializePage() override;

    DataSheetThread *datasheetThread();

protected:
    void finish();
    void changePage(int page);
    void addLog(const QString &str);

private:
    Datasheet *_datasheet;
    DataSheetThread *_thread;
    QLabel *_statusLabel;
    QLabel *_progressLabel;
    QProgressBar *_progressBar;
    QPlainTextEdit *_logger;
    int _pageStart;
    int _pageCount;
    bool _complete;
};

#endif  // DATASHEETPROCESSPAGE_H
