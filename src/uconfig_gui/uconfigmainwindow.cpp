#include "uconfigmainwindow.h"

#include "importer/pinlistimporter.h"

#include <QApplication>
#include <QScreen>
#include <QMenuBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QPushButton>

#include "pinruler/pinruler.h"
#include "pinruler/rulesparser.h"
#include "pinruler/rulesset.h"

UConfigMainWindow::UConfigMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createWidgets();
    createDocks();
    createToolbarsMenus();
    setWindowTitle("uConfig");

    resize(QApplication::primaryScreen()->size()*.7);

#if 0
    Datasheet datasheet;
    datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MM_GPM_revC.pdf");
    datasheet.analyse(0, 12);
    foreach (DatasheetPackage *package, datasheet.packages())
    {
        Component *component = package->toComponent();
        component->reorganizeToPackageStyle();
        _componentsTreeView->addComponent(component);
    }
    if (!datasheet.packages().empty())
    {
        selectComponent(_componentsTreeView->lib()->components()[0]);
    }
#endif
}

void UConfigMainWindow::importComponents()
{
    PinListImporter importer;
    importer.exec();


    foreach (DatasheetPackage *package, importer.datasheet()->packages())
    {
        Component *component = package->toComponent();
        component->reorganizeToPackageStyle();
        _componentsTreeView->addComponent(component);
    }
    if (!_componentsTreeView->lib()->components().empty())
    {
        selectComponent(_componentsTreeView->lib()->components()[0]);
    }
    _componentsTreeView->resizeColumnToContents(0);
    _componentsTreeView->resizeColumnToContents(1);
}

void UConfigMainWindow::saveLib()
{
    saveLibAs(_pathLib);
}

void UConfigMainWindow::saveLibAs(const QString &fileName)
{
    QString libFileName;

    if(fileName.isEmpty())
    {
        QFileDialog fileDialog(this);
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        fileDialog.setDefaultSuffix(".lib");
        fileDialog.setNameFilter(tr("Node project (*.lib)"));
        fileDialog.setWindowTitle(tr("Save Kicad library"));
        if (fileDialog.exec())
            libFileName = fileDialog.selectedFiles().first();
        if(fileName.isEmpty())
            return;
    }
    else
        libFileName = fileName;

    if(!libFileName.endsWith(".lib"))
        libFileName.append(".lib");
    _pathLib = libFileName;

    _componentsTreeView->lib()->saveTo(libFileName);
}

void UConfigMainWindow::selectComponent(Component *component)
{
    _componentsPinTableView->setComponent(component);
    _componentViewer->setComponent(component);
}

void UConfigMainWindow::organize()
{

    RulesSet ruleSet;
    RulesParser parser("../rules/tst.rule");
    parser.parse(&ruleSet);

    PinRuler ruler(&ruleSet);
    Component *component = _componentViewer->component();
    _componentViewer->scene()->clear();
    ruler.organize(component);
    _componentViewer->setComponent(component);
}

void UConfigMainWindow::createWidgets()
{
    _componentViewer = new ComponentViewer();

    _componentsPinTableView = new ComponentPinsTableView();

    connect(_componentViewer, &ComponentViewer::pinSelected, _componentsPinTableView, &ComponentPinsTableView::selectPin);
    connect(_componentsPinTableView, &ComponentPinsTableView::pinSelected, _componentViewer, &ComponentViewer::selectPin);

    QWidget *viewerContentLayout = new QWidget();
    QVBoxLayout *viewerLayout = new QVBoxLayout();
    QPushButton *button = new QPushButton("organize");
    connect(button, &QPushButton::clicked, this, &UConfigMainWindow::organize);
    viewerLayout->addWidget(button);
    viewerLayout->addWidget(_componentViewer);
    viewerContentLayout->setLayout(viewerLayout);

    _splitter = new QSplitter();
    _splitter->addWidget(_componentsPinTableView);
    _splitter->addWidget(viewerContentLayout);
    _splitter->setSizes(QList<int>()<<200<<200);

    setCentralWidget(_splitter);
    setStatusBar(new QStatusBar());
}

void UConfigMainWindow::createDocks()
{
    _componentsListDock = new QDockWidget(tr("Components list"), this);
    QWidget *componentsListContent = new QWidget(_componentsListDock);
    QLayout *componentsListLayout = new QVBoxLayout();
    _componentsTreeView = new ComponentLibTreeView();
    connect(_componentsTreeView, &ComponentLibTreeView::openedComponent, this, &UConfigMainWindow::selectComponent);
    componentsListLayout->addWidget(_componentsTreeView);
    componentsListContent->setLayout(componentsListLayout);
    _componentsListDock->setWidget(componentsListContent);
    addDockWidget(Qt::LeftDockWidgetArea, _componentsListDock);
}

void UConfigMainWindow::createToolbarsMenus()
{
    setMenuBar(new QMenuBar(this));
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(32, 32));
    addToolBar(toolBar);

    // ============= File =============
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openFileAction = new QAction(tr("&Import components..."),this);
    openFileAction->setStatusTip(tr("Import components from various format of files"));
    openFileAction->setShortcut(QKeySequence::Open);
    openFileAction->setIcon(QIcon(":/icons/img/import"));
    fileMenu->addAction(openFileAction);
    toolBar->addAction(openFileAction);
    connect(openFileAction, &QAction::triggered, this, &UConfigMainWindow::importComponents);

    QAction *saveFileAction = new QAction(tr("&Save lib"),this);
    saveFileAction->setStatusTip(tr("Save to Kicad components lib"));
    saveFileAction->setShortcut(QKeySequence::Save);
    saveFileAction->setIcon(QIcon(":/icons/img/save"));
    fileMenu->addAction(saveFileAction);
    toolBar->addAction(saveFileAction);
    connect(saveFileAction, &QAction::triggered, this, &UConfigMainWindow::saveLib);

    QAction *saveFileAsAction = new QAction(tr("Save lib &as..."),this);
    saveFileAsAction->setStatusTip(tr("Save to Kicad components lib with new name"));
    saveFileAsAction->setShortcut(QKeySequence::SaveAs);
    saveFileAsAction->setIcon(QIcon(":/icons/img/save-as"));
    fileMenu->addAction(saveFileAsAction);
    connect(saveFileAsAction, SIGNAL(triggered()), this, SLOT(saveLibAs()));

    fileMenu->addSeparator();
    QAction *exitAction = new QAction(tr("E&xit"),this);
    exitAction->setStatusTip(tr("Exits uConfig"));
    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // ============= Help =============
    QMenu *helpMenu = menuBar()->addMenu("&Help");

    QAction *aboutAction = new QAction("&About", this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
    helpMenu->addAction(aboutAction);

    QAction *aboutQtAction = new QAction("About &Qt", this);
    connect(aboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    helpMenu->addAction(aboutQtAction);
}

void UConfigMainWindow::about()
{
    QMessageBox::about(this,"uConfig v0", QString("Copyright (C) 2017 Robotips (<a href=\"https://robotips.fr\">robotips.fr</a>)<br>\
<br>\
This sofware is part of uConfig distribution. To check for new version, please visit <a href=\"https://github.com/Robotips/uConfig\">github.com/Robotips/uConfig</a><br>\
<br>\
Written by <a href=\"https://github.com/sebcaux\">Sébastien CAUX (sebcaux)</a><br>\
<br>\
uConfig is a free software: you can redistribute it and/or modify \
it under the terms of the GNU General Public License as published by \
the Free Software Foundation, either version 3 of the License, or \
(at your option) any later version.<br>\
<br>\
This program is distributed in the hope that it will be useful, \
but WITHOUT ANY WARRANTY; without even the implied warranty of \
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the \
GNU General Public License for more details.<br>\
<br>\
You should have received a copy of the GNU General Public License \
along with this program. If not, see <a href=\"http://www.gnu.org/licenses/\">www.gnu.org/licenses</a><br>\
<br>\
Build date: ") + __DATE__ + QString(" time: ")+__TIME__);
}
