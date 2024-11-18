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

PDFPage::PDFPage(Poppler::Page *page)
    : std::unique_ptr<Poppler::Page>(page)
{
    loadBoxes();
}

PDFPage::~PDFPage()
{
    for (PDFTextBox *textBox : qAsConst(_textBoxes))
    {
        delete textBox;
        textBox = nullptr;
    }
}

int PDFPage::numPage() const
{
    return get()->index();
}

QRect PDFPage::pageRect() const
{
    return QRect(QPoint(0, 0), get()->pageSize());
}

void PDFPage::loadBoxes()
{
    QString fullText;
    QRectF  fullBoundingRect;
    const auto& texts = get()->textList();
    for (Poppler::TextBox *ptextBox : texts)
    {
        bool isNumber = false;
        QString text = ptextBox->text();
        text.toInt(&isNumber);

        if(fullText.isEmpty() || isNumber)
        {
            _textBoxes.append(new PDFTextBox(text, ptextBox->boundingBox()));
        }
        else
        {
            fullBoundingRect = fullBoundingRect.united(ptextBox->boundingBox());
            fullText += text;

            if(ptextBox->nextWord() != nullptr)
            {
                if(!ptextBox->hasSpaceAfter())
                    fullText.append(QChar(' '));
            }
            else
            {
                _textBoxes.append(new PDFTextBox(fullText, fullBoundingRect.adjusted(-1, -1, 1, 1)));
                fullText.clear();
                fullBoundingRect = QRectF();
            }
        }
        delete ptextBox;
        ptextBox = nullptr;
    }
}

const QList<PDFTextBox *> &PDFPage::textBoxes() const
{
    return _textBoxes;
}
