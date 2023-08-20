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

#ifndef DATASHEET_H
#define DATASHEET_H

#include "pdf_extract_common.h"

#include <QImage>
#include <QObject>

#include "datasheetbox.h"
#include "datasheetpackage.h"
#include <kicad/model/component.h>

namespace Poppler
{
class Document;
class Page;
}  // namespace Poppler

class DATASHEET_EXTRACTOR_EXPORT Datasheet : public QObject
{
    Q_OBJECT
public:
    Datasheet();
    ~Datasheet() override;

    bool open(const QString &fileName);
    void close();

    void analyse(int pageBegin = -1, int pageEnd = -1);

    const QList<DatasheetPackage *> &packages() const;
    QList<Component *> components();

    QString name() const;

    bool debugEnabled() const;
    void setDebugEnabled(bool debug = true);

    bool forceEnabled() const;
    void setForceEnabled(bool force = true);

    int pageCount() const;
    QImage pageThumbnail(int numPage) const;

    void clean();

signals:
    void pageChanged(int page);
    void log(QString txt);

private:
    int pagePinDiagram(int pageStart, int pageEnd, bool *bgaStyle);

    void pinSearch(int numPage);

    QRectF toGlobalPos(const QRectF &rect, Poppler::Page *page, int pageNumber);
    QList<DatasheetPin *> extractPins(int numPage);
    QList<DatasheetBox *> _numbers;
    QList<DatasheetBox *> _labels;
    QList<DatasheetBox *> _proc_labels;
    QList<DatasheetBox *> _pack_labels;

    Poppler::Document *_doc;
    QString _name;

    QList<DatasheetPackage *> _packages;
    bool _debug;
    bool _force;
};

#endif  // DATASHEET_H
