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

#include "pdftextbox.h"

PDFTextBox::PDFTextBox(const QString& text, const QRectF &boundingRect)
    : _text(text),
      _boundingRect(boundingRect),
      _type(Text)
{
  if(isPadName())
  {
    _type = PDFTextBox::Pad;
  }
}

PDFTextBox::~PDFTextBox()
{
    for (PDFTextBox *textBox : _subBoxes)
    {
        delete textBox;
    }
}

const QList<PDFTextBox *> &PDFTextBox::subBoxes() const
{
    return _subBoxes;
}

const QString &PDFTextBox::text() const
{
    return _text;
}

const QRectF &PDFTextBox::boundingRect() const
{
    return _boundingRect;
}

bool PDFTextBox::isPadName() const
{
    bool okNumber = false;
    _text.toInt(&okNumber);
    if (okNumber)
    {
        return true;
    }
    // TODO add BGA pad name filter
    return false;
}

PDFTextBox::Type PDFTextBox::type() const
{
    return _type;
}
