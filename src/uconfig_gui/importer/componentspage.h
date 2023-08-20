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

#ifndef COMPONENTSPAGE_H
#define COMPONENTSPAGE_H

#include <QCheckBox>
#include <QLabel>
#include <QWizardPage>

#include <kicad/itemmodel/componentlibtreeview.h>

class ComponentsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit ComponentsPage();

    bool isComplete() const override;
    int nextId() const override;
    void initializePage() override;

protected:
    void finish();
    void changePage(int page);
    void addLog(QString str);
    bool validatePage() override;

private:
    QCheckBox *_checkAllBox;
    QLabel *_statusLabel;
    ComponentLibTreeView *_componentTreeView;
    Lib *_lib;
};

#endif  // COMPONENTSPAGE_H
