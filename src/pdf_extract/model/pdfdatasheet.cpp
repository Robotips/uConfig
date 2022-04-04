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

#include "pdfdatasheet.h"

#include <utility>

#include "controller/pdfloader.h"

PDFDatasheet::PDFDatasheet(QString fileName)
    : _fileName(std::move(fileName))
{
    _pdfLoader = new PDFLoader(this);
}

PDFDatasheet::~PDFDatasheet()
{
    delete _pdfLoader;
}

const QString &PDFDatasheet::fileName() const
{
    return _fileName;
}

const QString &PDFDatasheet::title() const
{
    return _title;
}

bool PDFDatasheet::loadPage(int numPage)
{
    if (numPage >= _pageCount || numPage < 0)
    {
        return false;
    }
    if (page(numPage) != nullptr)
    {
        return true;
    }

    PDFPage *page = new PDFPage(this, numPage);
    _pagesLoaded.insert(numPage, page);
    return _pdfLoader->loadPage(page);
}

int PDFDatasheet::pageCount() const
{
    return _pageCount;
}

int PDFDatasheet::loadedPageCount() const
{
    return _pagesLoaded.size();
}

PDFPage *PDFDatasheet::page(int numPage)
{
    QMap<int, PDFPage *>::const_iterator itFind = _pagesLoaded.constFind(numPage);
    if (itFind == _pagesLoaded.cend())
    {
        return nullptr;
    }
    return *itFind;
}

PDFLoader *PDFDatasheet::pdfLoader() const
{
    return _pdfLoader;
}
