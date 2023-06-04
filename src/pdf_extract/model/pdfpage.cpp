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

#include "pdfpage.h"
#include "controller/pdfloader.h"
#include "pdfdatasheet.h"

#include <poppler/qt5/poppler-qt5.h>

PDFPage::PDFPage(PDFDatasheet *datasheet, int numPage)
    : _datasheet(datasheet),
      _numPage(numPage),
      _boxesLoaded(false),
      _page(nullptr)
{
}

PDFPage::~PDFPage()
{
    for (PDFTextBox *textBox : _textBoxes)
    {
        delete textBox;
    }
    delete _page;
}

PDFDatasheet *PDFPage::datasheet() const
{
    return _datasheet;
}

int PDFPage::numPage() const
{
    return _numPage;
}

const QRect &PDFPage::pageRect() const
{
    return _pageRect;
}

const QImage &PDFPage::image() const
{
    return _image;
}

Poppler::Page *PDFPage::page() const
{
    return _page;
}

void PDFPage::loadBoxes()
{
    if (!_boxesLoaded)
    {
        _datasheet->pdfLoader()->loadBoxes(this);
    }
}

bool PDFPage::boxesLoaded() const
{
    return _boxesLoaded;
}

const QList<PDFTextBox *> &PDFPage::textBoxes() const
{
    return _textBoxes;
}
