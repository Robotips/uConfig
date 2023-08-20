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

#ifndef PDFDEBUGWIDGET_H
#define PDFDEBUGWIDGET_H

#include <pdf_extract_common.h>

#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include "model/pdfdatasheet.h"
#include "pdfdebugviewer.h"

class DATASHEET_EXTRACTOR_EXPORT PdfDebugWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PdfDebugWidget(QWidget *parent = nullptr);
    PdfDebugWidget(PDFDatasheet *datasheet, QWidget *parent = nullptr);

    PDFDatasheet *datasheet() const;
    void setDatasheet(PDFDatasheet *datasheet);

    PDFPage *currentPage() const;
    int currentNumPage() const;

signals:
    void pageChanged(int page);

public slots:
    void showPage(int page);
    void showPage(PDFPage *page);

    void previous();
    void next();

protected:
    PDFDatasheet *_datasheet;
    PDFPage *_currentPage;

    void createWidgets();
    PdfDebugViewer *_viewer;
    QAction *_ationPrev;
    QAction *_ationNext;
    QLineEdit *_pageLineEdit;
    QLabel *_pageLabel;
};

#endif  // PDFDEBUGWIDGET_H
