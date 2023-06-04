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

#include "pdfdebugwidget.h"

#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

PdfDebugWidget::PdfDebugWidget(QWidget *parent)
    : QWidget(parent),
      _datasheet(nullptr)
{
    _currentPage = nullptr;
    createWidgets();
}

PdfDebugWidget::PdfDebugWidget(PDFDatasheet *datasheet, QWidget *parent)
    : QWidget(parent)
{
    _currentPage = nullptr;
    createWidgets();
    setDatasheet(datasheet);
}

PDFDatasheet *PdfDebugWidget::datasheet() const
{
    return _datasheet;
}

void PdfDebugWidget::setDatasheet(PDFDatasheet *datasheet)
{
    _datasheet = datasheet;

    if (_datasheet != nullptr)
    {
        _pageLabel->setText(QString(" / %1").arg(_datasheet->pageCount()));
    }
}

void PdfDebugWidget::showPage(int page)
{
    if (_datasheet == nullptr)
    {
        return;
    }
    if (!_datasheet->loadPage(page))
    {
        return;
    }

    PDFPage *pdfPage = _datasheet->page(page);
    if (pdfPage == nullptr)
    {
        return;
    }
    showPage(pdfPage);
}

void PdfDebugWidget::showPage(PDFPage *page)
{
    if (page->datasheet() != _datasheet)
    {
        setDatasheet(page->datasheet());
    }

    if (!page->boxesLoaded())
    {
        page->loadBoxes();
    }

    _currentPage = page;
    _viewer->setPage(page);
    _pageLineEdit->setText(QString::number(_currentPage->numPage() + 1));
    emit pageChanged(_currentPage->numPage());
}

void PdfDebugWidget::previous()
{
    showPage(currentNumPage() - 1);
}

void PdfDebugWidget::next()
{
    showPage(currentNumPage() + 1);
}

PDFPage *PdfDebugWidget::currentPage() const
{
    return _currentPage;
}

int PdfDebugWidget::currentNumPage() const
{
    if (_currentPage == nullptr)
    {
        return 0;
    }
    return _currentPage->numPage();
}

void PdfDebugWidget::createWidgets()
{
    _viewer = new PdfDebugViewer();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    QToolBar *toolBar = new QToolBar("ToolBar", this);
    toolBar->setIconSize(QSize(30, 30));

    _ationPrev = new QAction(this);
    _ationPrev->setText(tr("<"));
    _ationPrev->setShortcut(QKeySequence::MoveToPreviousPage);
    connect(_ationPrev, &QAction::triggered, this, &PdfDebugWidget::previous);
    toolBar->addAction(_ationPrev);
    addAction(_ationPrev);

    _pageLineEdit = new QLineEdit();
    _pageLineEdit->setMaximumWidth(50);
    toolBar->addWidget(_pageLineEdit);

    _pageLabel = new QLabel();
    toolBar->addWidget(_pageLabel);

    _ationNext = new QAction(this);
    _ationNext->setText(tr(">"));
    _ationNext->setShortcut(QKeySequence::MoveToNextPage);
    connect(_ationNext, &QAction::triggered, this, &PdfDebugWidget::next);
    toolBar->addAction(_ationNext);
    addAction(_ationNext);

    layout->addWidget(toolBar);
    layout->addWidget(_viewer);

    setLayout(layout);
}
