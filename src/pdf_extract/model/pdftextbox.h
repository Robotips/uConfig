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

#ifndef PDFTEXTBOX_H
#define PDFTEXTBOX_H

#include <pdf_extract_common.h>

class PDFPage;

#include <QList>
#include <QRect>
#include <QString>

class DATASHEET_EXTRACTOR_EXPORT PDFTextBox
{
public:
    PDFTextBox(QString text, const QRectF &boundingRect);
    ~PDFTextBox();

    const QString &text() const;
    const QRectF &boundingRect() const;

    bool isPadName() const;
    enum Type
    {
        Text,
        SubText,
        Pad
    };
    Type type() const;

    const QList<PDFTextBox *> &subBoxes() const;
    PDFTextBox *parentBox() const;
    PDFPage *page() const;

protected:
    QString _text;
    QRectF _boundingRect;
    Type _type;

    QList<PDFTextBox *> _subBoxes;
    PDFPage *_page;
    PDFTextBox *_parentBox;
    friend class PDFLoader;
};

#endif  // PDFTEXTBOX_H
