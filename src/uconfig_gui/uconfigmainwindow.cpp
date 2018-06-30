#include "uconfigmainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QMenuBar>
#include <QToolBar>
#include <QWidgetAction>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QDir>
#include <QMimeData>
#include <QUrl>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QTabWidget>
#include <QPixmap>
#include <QScrollArea>

#include <QDebug>

#include "pinruler/pinruler.h"
#include "pinruler/rulesparser.h"
#include "pinruler/rulesset.h"

UConfigMainWindow::UConfigMainWindow(UConfigProject *project)
    : _project(project)
{
    setWindowIcon(QIcon(":/icons/img/uConfig.ico"));
    createWidgets();
    createDocks();
    createToolbarsMenus();
    setWindowTitle("uConfig");
    reloadRuleSetList();
    setAcceptDrops(true);

    connect(_project, &UConfigProject::libChanged, _componentsTreeView, &ComponentLibTreeView::setLib);
    connect(_project, &UConfigProject::libChanged, this, &UConfigMainWindow::setTitle);
    connect(_project, &UConfigProject::activeComponentChange, _componentsTreeView, &ComponentLibTreeView::setActiveComponent);
    connect(_project, &UConfigProject::activeComponentChange, _componentInfosEditor, &ComponentInfosEditor::setComponent);
    connect(_project, &UConfigProject::activeComponentChange, _pinListEditor, &PinListEditor::setComponent);
    connect(_project, &UConfigProject::activeComponentChange, _componentWidget, &ComponentWidget::setComponent);

    resize(QApplication::primaryScreen()->size()*.7);
    setTitle();
}

UConfigMainWindow::~UConfigMainWindow()
{
}

void UConfigMainWindow::setTitle()
{
    setWindowTitle(_project->libName() + " | uConfig");
}

void UConfigMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasUrls())
        return;

    QString fileName = event->mimeData()->urls().first().toLocalFile();
    if (fileName.endsWith(".pdf") || fileName.endsWith(".lib"))
        event->accept();
}

void UConfigMainWindow::dropEvent(QDropEvent *event)
{
    event->accept();

    foreach (const QUrl &url, event->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
        _project->importComponents(fileName);
    }
}

void UConfigMainWindow::organize(QString ruleSetName)
{
    disconnect(_kssEditor, &KssEditor::textChanged, this, &UConfigMainWindow::updateRules);
    if (ruleSetName == "package")
    {
        if (!_componentWidget->component())
            return;
        Component *component = _componentWidget->component();
        _componentWidget->setComponent(Q_NULLPTR);
        component->reorganizeToPackageStyle();
        _componentWidget->setComponent(component);
        _kssEditor->clear();
        return;
    }
    RulesSet ruleSet;
    RulesParser parser(QString("../rules/%1.kss").arg(ruleSetName));
    QFile kssFile(QString("../rules/%1.kss").arg(ruleSetName));
    kssFile.open(QIODevice::ReadOnly | QIODevice::Text);
    _kssEditor->clear();
    _kssEditor->appendPlainText(kssFile.readAll());
    connect(_kssEditor, &KssEditor::textChanged, this, &UConfigMainWindow::updateRules);
    parser.parse(&ruleSet);

    if (_componentWidget->component())
    {
        PinRuler ruler(&ruleSet);
        Component *component = _componentWidget->component();
        _componentWidget->setComponent(Q_NULLPTR);
        ruler.organize(component);
        _componentWidget->setComponent(component);
    }
}

void UConfigMainWindow::updateRules()
{
    RulesSet ruleSet;
    RulesParser parser;
    parser.setData(_kssEditor->document()->toPlainText());
    if (!parser.parse(&ruleSet))
        return;

    PinRuler ruler(&ruleSet);
    if (_componentWidget->component())
    {
        Component *component = _componentWidget->component();
        _componentWidget->setComponent(Q_NULLPTR);
        ruler.organize(component);
        _componentWidget->setComponent(component);
    }
}

void UConfigMainWindow::reloadRuleSetList()
{
    _ruleComboBox->clear();
    _ruleComboBox->addItem("package");
    QDir dir("../rules/");
    foreach (const QFileInfo &ruleInfo, dir.entryInfoList(QStringList()<<"*.kss", QDir::NoDotAndDotDot | QDir::Files))
    {
        _ruleComboBox->addItem(ruleInfo.baseName());
    }
}

void UConfigMainWindow::createWidgets()
{
    _splitterEditor = new QSplitter(Qt::Vertical);
    _pinListEditor = new PinListEditor();
    _kssEditor = new KssEditor();
    _splitterEditor->addWidget(_pinListEditor);
    _splitterEditor->addWidget(_kssEditor);
    _splitterEditor->setSizes(QList<int>()<<200<<200);

    QTabWidget *tabWidget = new QTabWidget();
    _componentWidget = new ComponentWidget(this);
    _pdfDebug = new QLabel();
    _pdfDebug->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    _pdfDebug->setScaledContents(true);
    QScrollArea *pdfDebugArea = new QScrollArea();
    pdfDebugArea->setWidget(_pdfDebug);
    pdfDebugArea->setWidgetResizable(true);
    tabWidget->addTab(_componentWidget, "component");
    tabWidget->addTab(pdfDebugArea, "pdf debugger");

    _splitter = new QSplitter();
    _splitter->addWidget(_splitterEditor);
    _splitter->addWidget(tabWidget);
    _splitter->setSizes(QList<int>()<<200<<200);

    setCentralWidget(_splitter);
    setStatusBar(new QStatusBar());

    connect(_componentWidget->viewer(), &ComponentViewer::droppedFile,
            _project, &UConfigProject::importComponents);

    connect(_componentWidget->viewer(), &ComponentViewer::pinsSelected,
            _pinListEditor->tableView(), &ComponentPinsTableView::selectPins);
    connect(_pinListEditor->tableView(), &ComponentPinsTableView::pinSelected,
            _componentWidget->viewer(), &ComponentViewer::selectPins);
    connect(_pinListEditor->tableView()->model(), &ComponentPinsItemModel::pinModified,
            _componentWidget->viewer(), &ComponentViewer::updatePin);
}

void UConfigMainWindow::createDocks()
{
    _componentsListDock = new QDockWidget(tr("Components list"), this);
    QWidget *componentsListContent = new QWidget(_componentsListDock);
    QLayout *componentsListLayout = new QVBoxLayout();
    componentsListLayout->setContentsMargins(5, 5, 5, 5);
    _componentsTreeView = new ComponentLibTreeView();
    _componentsTreeView->setEditMode(true);
    connect(_componentsTreeView, &ComponentLibTreeView::openedComponent, _project, &UConfigProject::selectComponent);
    componentsListLayout->addWidget(_componentsTreeView);
    componentsListContent->setLayout(componentsListLayout);
    _componentsListDock->setWidget(componentsListContent);
    addDockWidget(Qt::LeftDockWidgetArea, _componentsListDock);

    _componentInfosDock = new QDockWidget(tr("Component infos"), this);
    QWidget *componentInfosContent = new QWidget(_componentsListDock);
    QLayout *componentInfosLayout = new QVBoxLayout();
    componentInfosLayout->setContentsMargins(5, 5, 5, 5);
    _componentInfosEditor = new ComponentInfosEditor(_project);
    componentInfosLayout->addWidget(_componentInfosEditor);
    componentInfosContent->setLayout(componentInfosLayout);
    _componentInfosDock->setWidget(componentInfosContent);
    addDockWidget(Qt::LeftDockWidgetArea, _componentInfosDock);
}

void UConfigMainWindow::createToolbarsMenus()
{
    setMenuBar(new QMenuBar(this));
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setIconSize(QSize(32, 32));
    addToolBar(toolBar);

    // ============= File =============
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *newFileAction = new QAction(tr("&New library..."), this);
    newFileAction->setStatusTip(tr("Create a new library of components"));
    newFileAction->setShortcut(QKeySequence::New);
    newFileAction->setIcon(QIcon(":/icons/img/new"));
    fileMenu->addAction(newFileAction);
    toolBar->addAction(newFileAction);
    connect(newFileAction, &QAction::triggered, _project, &UConfigProject::newLib);

    QAction *openFileAction = new QAction(tr("&Open library..."), this);
    openFileAction->setStatusTip(tr("Opens an existing library"));
    openFileAction->setShortcut(QKeySequence::Open);
    openFileAction->setIcon(QIcon(":/icons/img/open"));
    fileMenu->addAction(openFileAction);
    toolBar->addAction(openFileAction);
    connect(openFileAction, SIGNAL(triggered()), _project, SLOT(openLib()));

    QAction *importFileAction = new QAction(tr("&Import components..."), this);
    importFileAction->setStatusTip(tr("Imports components from various format of files"));
    importFileAction->setShortcut(QKeySequence("Ctrl+I"));
    importFileAction->setIcon(QIcon(":/icons/img/import"));
    fileMenu->addAction(importFileAction);
    toolBar->addAction(importFileAction);
    connect(importFileAction, SIGNAL(triggered()), _project, SLOT(importComponents()));

    QAction *saveFileAction = new QAction(tr("&Save lib"), this);
    saveFileAction->setStatusTip(tr("Save to Kicad components lib"));
    saveFileAction->setShortcut(QKeySequence::Save);
    saveFileAction->setIcon(QIcon(":/icons/img/save"));
    fileMenu->addAction(saveFileAction);
    toolBar->addAction(saveFileAction);
    connect(saveFileAction, &QAction::triggered, _project, &UConfigProject::saveLib);

    QAction *saveFileAsAction = new QAction(tr("Save lib &as..."), this);
    saveFileAsAction->setStatusTip(tr("Save to Kicad components lib with new name"));
    saveFileAsAction->setShortcut(QKeySequence::SaveAs);
    saveFileAsAction->setIcon(QIcon(":/icons/img/save-as"));
    fileMenu->addAction(saveFileAsAction);
    connect(saveFileAsAction, SIGNAL(triggered()), _project, SLOT(saveLibAs()));

    fileMenu->addSeparator();
    QAction *exitAction = new QAction(tr("E&xit"), this);
    exitAction->setStatusTip(tr("Exits uConfig"));
    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // rules
    toolBar->addSeparator();
    _ruleComboBox = new QComboBox();
    connect(_ruleComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(organize(QString)));
    toolBar->addWidget(new QLabel(tr(" pin ruler: ")));
    QAction *actionRuleCombox = toolBar->addWidget(_ruleComboBox);
    actionRuleCombox->setStatusTip(tr("Change the rule for pin organisation"));

    // ============= View =============
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    QAction *gridAction = new QAction(tr("&Grid"), this);
    gridAction->setCheckable(true);
    gridAction->setChecked(true);
    gridAction->setShortcut(QKeySequence("Ctrl+G"));
    connect(gridAction, SIGNAL(triggered(bool)), _componentWidget->viewer(), SLOT(setGridVisible(bool)));
    viewMenu->addAction(gridAction);

    viewMenu->addSeparator();
    QAction *componentsListDockAction = _componentsListDock->toggleViewAction();
    viewMenu->addAction(componentsListDockAction);

    QAction *_componentInfosDockAction = _componentInfosDock->toggleViewAction();
    viewMenu->addAction(_componentInfosDockAction);

    // ============= Help =============
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction *aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
    helpMenu->addAction(aboutAction);

    QAction *aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
    helpMenu->addAction(aboutQtAction);
}

void UConfigMainWindow::about()
{
    QMessageBox::about(this, "uConfig v0", QString("Copyright (C) 2018 Robotips (<a href=\"https://robotips.fr\">robotips.fr</a>)<br>\
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
