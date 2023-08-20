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

#ifndef UCONFIGMAINWINDOW_H
#define UCONFIGMAINWINDOW_H

#include <QMainWindow>

#include <QComboBox>
#include <QDockWidget>
#include <QLabel>
#include <QSplitter>
#include <QTableView>
#include <QTreeView>

#include "project/uconfigproject.h"

#include "componentinfoseditor.h"
#include "itemmodel/componentlibtreeview.h"
#include "itemmodel/pinlisteditor.h"
#include "ksseditor/ksseditor.h"
#include "viewer/componentwidget.h"

class UConfigMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    UConfigMainWindow(UConfigProject *project);
    ~UConfigMainWindow() override;

protected slots:
    void setTitle();

    void organize();
    void updateRules();
    void reloadRuleSetList();
    void setActiveComponent(Component *component);

    void about();

protected:
    UConfigProject *_project;

    void createWidgets();
    QSplitter *_splitter;
    ComponentLibTreeView *_componentsTreeView;
    ComponentInfosEditor *_componentInfosEditor;
    PinListEditor *_pinListEditor;
    ComponentWidget *_componentWidget;
    QComboBox *_ruleComboBox;
    QLabel *_pdfDebug;

    QSplitter *_splitterEditor;
    KssEditor *_kssEditor;

    void createDocks();
    QDockWidget *_componentsListDock;
    QDockWidget *_componentInfosDock;

    void writeSettings();
    void readSettings();
    QList<QAction *> _oldProjectsActions;

    void createToolbarsMenus();

protected slots:
    void openRecentFile();
    void updateOldProjects();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    // QObject interface
public:
    bool event(QEvent *event) override;
};

#endif  // UCONFIGMAINWINDOW_H
