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

#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QLabel>
#include <QLineEdit>
#include <QMap>
#include <QSettings>
#include <QWizardPage>

#include "pinlistimporter.h"

class FilePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit FilePage();

    bool isComplete() const override;
    int nextId() const override;
    void initializePage() override;

    Q_PROPERTY(QString file READ file)
    QString file() const;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:
    void fileExplore();
    void checkEntry(const QString &text);

private:
    bool _complete;
    QString _file;
    QString _fileTitle;
    QLineEdit *_fileEdit;
    QStringList _suffixes;
    QSettings _settings;
    void setFile(const QString &file);
    static QMap<PinListImporter::ImportType, QString> _importTypeSettingsKeymap;
};

#endif  // FILEPAGE_H
