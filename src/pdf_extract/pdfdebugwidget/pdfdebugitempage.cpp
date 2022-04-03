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

#include "pdfdebugitempage.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <poppler/qt5/poppler-form.h>
#include <poppler/qt5/poppler-qt5.h>

#include "pdfdebugitemtextbox.h"

PdfDebugItemPage::PdfDebugItemPage(PDFPage *page)
{
    _page = page;

    for (PDFTextBox *textBox : _page->textBoxes())
    {
        new PdfDebugItemTextBox(textBox, this);
    }
}

PDFPage *PdfDebugItemPage::page() const
{
    return _page;
}

QRectF PdfDebugItemPage::boundingRect() const
{
    return _page->pageRect().adjusted(-10, -10, 10, 10);
}

void PdfDebugItemPage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setClipRect(option->exposedRect);

    painter->setPen(Qt::black);

    _page->page()->renderToPainter(painter, 72.0, 72.0, 0, 0, _page->pageRect().width(), _page->pageRect().width(), Poppler::Page::Rotate0);

    // const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    // QImage image = _page->page()->renderToImage(72.0 * lod, 72.0 * lod, 0 ,0);
    // painter->drawImage(_page->pageRect(), image);

    painter->drawRect(_page->pageRect());
}
