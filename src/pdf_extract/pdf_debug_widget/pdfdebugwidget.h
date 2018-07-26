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

#ifndef PDFDEBUGWIDGET_H
#define PDFDEBUGWIDGET_H

#include "pdf_extract_common.h"

#include <QWidget>

class DATASHEET_EXTRACTOR_EXPORT PdfDebugWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PdfDebugWidget(QWidget *parent = Q_NULLPTR);

signals:

public slots:
};

#endif // PDFDEBUGWIDGET_H
