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

#ifndef PDFPAGE_H
#define PDFPAGE_H

#include <pdf_extract_common.h>

#include <memory>
#include <poppler/qt5/poppler-qt5.h>

#include "pdftextbox.h"

#include <QImage>
#include <QList>
#include <QRect>

class DATASHEET_EXTRACTOR_EXPORT PDFPage : public std::unique_ptr<Poppler::Page>
{
public:
    PDFPage(Poppler::Page *page);
    ~PDFPage();

    int numPage() const;
    QRect pageRect() const;

    const QList<PDFTextBox *> &textBoxes() const;

protected:
    void loadBoxes();

    QList<PDFTextBox *> _textBoxes;
};

#endif  // PDFPAGE_H
