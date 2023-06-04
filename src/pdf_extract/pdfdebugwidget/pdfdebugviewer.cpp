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

#include "pdfdebugviewer.h"

#include <QMouseEvent>
#include <qmath.h>

PdfDebugViewer::PdfDebugViewer(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(new QGraphicsScene(this));

    setDragMode(QGraphicsView::ScrollHandDrag);
    setCursor(Qt::ArrowCursor);

    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    setRenderHint(QPainter::TextAntialiasing, true);
}

void PdfDebugViewer::setPage(PDFPage *page)
{
    _page = page;
    scene()->clear();

    if (page != nullptr)
    {
        PdfDebugItemPage *pageItem = new PdfDebugItemPage(_page);
        scene()->addItem(pageItem);
    }
}

PDFPage *PdfDebugViewer::page() const
{
    return _page;
}

void PdfDebugViewer::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->angleDelta().x() / 8;
    int numSteps = numDegrees / 15;

    double mscale = qPow(1.25, numSteps);

    if (transform().m11() < 0.5 && mscale < 1)
    {
        return;
    }
    if (transform().m11() > 4 && mscale > 1)
    {
        return;
    }

    scale(mscale, mscale);
}
