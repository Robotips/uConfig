/**
 ** This file is part of the uConfig project.
 ** Copyright 2018 Robotips sebastien.caux@robotips.fr.
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

#ifndef KSSEDITOR_H
#define KSSEDITOR_H

#include "kicad_global.h"

#include <QPlainTextEdit>

#include "ksssyntax.h"
#include "ksseditormargin.h"

class KICAD_EXPORT KssEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit KssEditor(QWidget *parent = Q_NULLPTR);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
protected:
    KSSSyntax *_syntax;

private slots:
    void updateLineNumberAreaWidth(int);
    void updateLineNumberArea(const QRect &, int);
    void highlightCurrentLine();

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    KssEditorMargin *_kssEditorMargin;
    int _lineError;
};

#endif // KSSEDITOR_H
