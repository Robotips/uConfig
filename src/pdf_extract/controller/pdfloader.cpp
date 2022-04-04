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

#include "pdfloader.h"

#include <poppler/qt5/poppler-form.h>
#include <poppler/qt5/poppler-qt5.h>

#include <QDebug>

PDFLoader::PDFLoader(PDFDatasheet *pdfDatasheet)
    : _pdfDatasheet(pdfDatasheet)
{
    _document = Poppler::Document::load(_pdfDatasheet->_fileName);
    _pdfDatasheet->_pageCount = _document->numPages();
    _pdfDatasheet->_title = _document->info("Title");

    _document->setRenderBackend(Poppler::Document::ArthurBackend);
    _document->setRenderHint(Poppler::Document::Antialiasing, true);
    _document->setRenderHint(Poppler::Document::TextAntialiasing, true);
}

PDFLoader::~PDFLoader()
{
    delete _document;
}

bool PDFLoader::loadPage(PDFPage *pdfPage)
{
    if (pdfPage->numPage() >= _document->numPages())
    {
        return false;
    }

    Poppler::Page *page = _document->page(pdfPage->numPage());
    if (page == nullptr)
    {
        return false;
    }
    pdfPage->_page = page;
    pdfPage->_pageRect = QRect(QPoint(0, 0), page->pageSize());

    return true;
}

void PDFLoader::loadBoxes(PDFPage *pdfPage)
{
    PDFTextBox *parentTextBox = nullptr;
    for (Poppler::TextBox *ptextBox : pdfPage->page()->textList())
    {
        PDFTextBox *textBox = new PDFTextBox(ptextBox->text(), ptextBox->boundingBox());
        textBox->_page = pdfPage;

        bool padName = textBox->isPadName();
        if (padName)
        {
            textBox->_type = PDFTextBox::Pad;
        }
        if (parentTextBox == nullptr)
        {
            if (ptextBox->nextWord() == nullptr || padName)
            {
                pdfPage->_textBoxes.append(textBox);
            }
            else
            {
                parentTextBox = new PDFTextBox(QString(), ptextBox->boundingBox());
                parentTextBox->_page = pdfPage;
                if (ptextBox->hasSpaceAfter())
                {
                    textBox->_text.append(QChar(' '));
                }
                textBox->_parentBox = parentTextBox;
                textBox->_type = PDFTextBox::SubText;
                parentTextBox->_subBoxes.append(textBox);
            }
        }
        else
        {
            if (padName)
            {
                pdfPage->_textBoxes.append(textBox);
            }
            else
            {
                textBox->_parentBox = parentTextBox;
                textBox->_type = PDFTextBox::SubText;
                parentTextBox->_subBoxes.append(textBox);
            }
            if (ptextBox->nextWord() == nullptr || padName)
            {
                QRectF boundingRect;
                QString text;
                for (PDFTextBox *subBox : parentTextBox->subBoxes())
                {
                    text.append(subBox->text());
                    boundingRect = boundingRect.united(subBox->boundingRect());
                }
                parentTextBox->_text = text;
                parentTextBox->_boundingRect = boundingRect.adjusted(-1, -1, 1, 1);

                pdfPage->_textBoxes.append(parentTextBox);
                parentTextBox = nullptr;
            }
            else
            {
                if (ptextBox->hasSpaceAfter())
                {
                    textBox->_text.append(QChar(' '));
                }
            }
        }
        delete ptextBox;
    }
    pdfPage->_boxesLoaded = true;
}
