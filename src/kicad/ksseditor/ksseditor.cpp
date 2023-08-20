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

#include "ksseditor.h"

#include <QFontMetrics>
#include <QPainter>

KssEditor::KssEditor(QWidget *parent)
    : QPlainTextEdit(parent)
{
    _syntax = new KSSSyntax(this->document());

    _kssEditorMargin = new KssEditorMargin(this);
    connect(this, &KssEditor::blockCountChanged, this, &KssEditor::updateLineNumberAreaWidth);
    connect(this, &KssEditor::updateRequest, this, &KssEditor::updateLineNumberArea);
    connect(this, &KssEditor::cursorPositionChanged, this, &KssEditor::updateExtraSelection);

    QFont font("Consolas");
    font.setPixelSize(12);
    font.setStyleHint(QFont::Monospace);
    setFont(font);

    _lineError = -1;
    updateExtraSelection();
}

void KssEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(_kssEditorMargin);
    painter.fillRect(event->rect(), Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());
    QFont font = painter.font();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            font.setBold(blockNumber == textCursor().blockNumber());
            painter.setFont(font);
            painter.drawText(0, top, _kssEditorMargin->width() - 5, fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

int KssEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }
#if QT_VERSION > QT_VERSION_CHECK(5, 10, 0)
    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
#else
    int space = 10 + fontMetrics().width(QLatin1Char('9')) * digits;
#endif

    return space;
}

void KssEditor::setLineError(int lineError)
{
    _lineError = lineError;
    updateExtraSelection();
}

int KssEditor::lineError() const
{
    return _lineError;
}

void KssEditor::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void KssEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy != 0)
    {
        _kssEditorMargin->scroll(0, dy);
    }
    else
    {
        _kssEditorMargin->update(0, rect.y(), _kssEditorMargin->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

void KssEditor::dragEnterEvent(QDragEnterEvent *event)
{
    event->ignore();
}

void KssEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    _kssEditorMargin->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void KssEditor::updateExtraSelection()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    QTextEdit::ExtraSelection selection;

    // highligh current line
    QColor lineColor = QColor(Qt::yellow).lighter(180);
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);

    // highligh error line
    if (_lineError > 0)
    {
        QTextCursor cursorError(document()->findBlockByLineNumber(_lineError - 1));
        QColor errorColor = QColor(Qt::red).lighter(180);
        selection.format.setBackground(errorColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = cursorError;
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}
