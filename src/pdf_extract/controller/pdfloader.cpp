/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
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

#include "pdfloader.h"

#include <poppler/qt5/poppler-qt5.h>
#include <poppler/qt5/poppler-form.h>

PDFLoader::PDFLoader(PDFDatasheet *pdfDatasheet)
    : _pdfDatasheet(pdfDatasheet)
{
    _document = Poppler::Document::load(_pdfDatasheet->_fileName);
    _pdfDatasheet->_pageCount = _document->numPages();
    _pdfDatasheet->_title = _document->info("Title");
}

bool PDFLoader::loadPage(PDFPage *pdfPage)
{
    if (pdfPage->numPage() >= _document->numPages())
        return false;

    Poppler::Page *page = _document->page(pdfPage->numPage());
    if (page == Q_NULLPTR)
        return false;
    pdfPage->_page = page;

    pdfPage->_pageRect = QRect(QPoint(0, 0), page->pageSize());
    int res = 4;
    pdfPage->_image = page->renderToImage(
        72.0 * res, 72.0 * res, 0, 0,
        pdfPage->_pageRect.width() * res, pdfPage->_pageRect.height() * res);

    return true;
}
