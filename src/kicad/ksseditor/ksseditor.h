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

#ifndef KSSEDITOR_H
#define KSSEDITOR_H

#include <QPlainTextEdit>

#include "ksseditormargin.h"
#include "ksssyntax.h"

class KICAD_EXPORT KssEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit KssEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setLineError(int lineError);
    int lineError() const;

protected:
    KSSSyntax *_syntax;

private slots:
    void updateLineNumberAreaWidth(int);
    void updateLineNumberArea(const QRect &, int);
    void updateExtraSelection();

    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    KssEditorMargin *_kssEditorMargin;
    int _lineError;
};

#endif  // KSSEDITOR_H
