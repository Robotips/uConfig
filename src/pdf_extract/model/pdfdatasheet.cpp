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

PDFDatasheet::PDFDatasheet(const QString& fileName)
    : std::unique_ptr<Poppler::Document>(Poppler::Document::load(fileName)),
    _fileName(fileName)
{
    if(*this)
    {
        get()->setRenderBackend(Poppler::Document::ArthurBackend);
        get()->setRenderHint(Poppler::Document::Antialiasing, true);
        get()->setRenderHint(Poppler::Document::TextAntialiasing, true);
    }
}

PDFDatasheet::~PDFDatasheet()
{
}

const QString &PDFDatasheet::fileName() const
{
    return _fileName;
}

QString PDFDatasheet::title() const
{
    return get()->info("Title");
}

bool PDFDatasheet::loadPage(int numPage)
{
    if (numPage >= pageCount() || numPage < 0)
    {
        return false;
    }

    PDFPage *pdfPage = page(numPage);

    if (pdfPage == nullptr)
    {
        pdfPage = new PDFPage(get()->page(numPage));
        _pagesLoaded.insert(numPage, pdfPage);
    }

    return pdfPage->numPage() < pageCount();
}

int PDFDatasheet::pageCount() const
{
    return get()->numPages();
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
