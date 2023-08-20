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

#ifndef PDFDEBUGVIEWER_H
#define PDFDEBUGVIEWER_H

#include <pdf_extract_common.h>

#include <QGraphicsView>

#include "model/pdfpage.h"
#include "pdfdebugitempage.h"

class DATASHEET_EXTRACTOR_EXPORT PdfDebugViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PdfDebugViewer(QWidget *parent = nullptr);

    PDFPage *page() const;

signals:

protected:
    void wheelEvent(QWheelEvent *event) override;

public slots:
    void setPage(PDFPage *page);

protected:
    PDFPage *_page;
};

#endif  // PDFDEBUGVIEWER_H
