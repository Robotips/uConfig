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

#ifndef PDFDATASHEET_H
#define PDFDATASHEET_H

#include <pdf_extract_common.h>

#include "pdfpage.h"

#include <QMap>
#include <QString>

class PDFLoader;

class DATASHEET_EXTRACTOR_EXPORT PDFDatasheet
{
public:
    PDFDatasheet(QString fileName);
    ~PDFDatasheet();

    const QString &fileName() const;
    const QString &title() const;

    bool loadPage(int numPage);

    int pageCount() const;
    int loadedPageCount() const;
    PDFPage *page(int numPage);

    PDFLoader *pdfLoader() const;

protected:
    int _pageCount;
    QMap<int, PDFPage *> _pagesLoaded;
    QString _fileName;
    QString _title;

    friend class PDFLoader;
    PDFLoader *_pdfLoader;
};

#endif  // PDFDATASHEET_H
