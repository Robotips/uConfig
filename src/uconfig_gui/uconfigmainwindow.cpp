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

#include "uconfigmainwindow.h"

#include <QApplication>
#include <QDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPixmap>
#include <QScreen>
#include <QScrollArea>
#include <QSettings>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidgetAction>

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
    setWindowTitle(tr("uConfig"));
    reloadRuleSetList();
    setAcceptDrops(true);

    connect(_project, &UConfigProject::libChanged, _componentsTreeView, &ComponentLibTreeView::setLib);
    connect(_project, &UConfigProject::libChanged, this, &UConfigMainWindow::setTitle);
    connect(_project, &UConfigProject::oldProjectChanged, this, &UConfigMainWindow::updateOldProjects);
    connect(_project, &UConfigProject::activeComponentChange, this, &UConfigMainWindow::setActiveComponent);
    connect(_project, &UConfigProject::activeComponentChange, _componentsTreeView, &ComponentLibTreeView::setActiveComponent);
    connect(_project, &UConfigProject::activeComponentChange, _componentInfosEditor, &ComponentInfosEditor::setComponent);
    connect(_project, &UConfigProject::activeComponentChange, _pinListEditor, &PinListEditor::setComponent);
    connect(_project, &UConfigProject::activeComponentChange, _componentWidget, &ComponentWidget::setComponent);
    connect(_project, &UConfigProject::componentChange, _componentsTreeView, &ComponentLibTreeView::updateComponent);

    resize(QApplication::primaryScreen()->size() * .7);
    setTitle();

    readSettings();
}

UConfigMainWindow::~UConfigMainWindow()
{
}

void UConfigMainWindow::setTitle()
{
    setWindowTitle(_project->libName() + tr(" | uConfig"));
}

void UConfigMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasUrls())
    {
        return;
    }

    QString fileName = event->mimeData()->urls().first().toLocalFile();
    if (fileName.endsWith(".pdf", Qt::CaseInsensitive) || fileName.endsWith(".lib", Qt::CaseInsensitive) || fileName.endsWith(".csv", Qt::CaseInsensitive)
        || fileName.endsWith(".txt", Qt::CaseInsensitive))
    {
        event->accept();
    }
}

void UConfigMainWindow::dropEvent(QDropEvent *event)
{
    event->accept();

    for (const QUrl &url : event->mimeData()->urls())
    {
        QString fileName = url.toLocalFile();
        _project->importComponents(fileName);
    }
}

void UConfigMainWindow::organize()
{
    const QString &ruleSetName = _ruleComboBox->currentText();
    disconnect(_kssEditor, &KssEditor::textChanged, this, &UConfigMainWindow::updateRules);
    if (ruleSetName == "package")
    {
        if (_componentWidget->component() == nullptr)
        {
            return;
        }
        Component *component = _componentWidget->component();
        _componentWidget->setComponent(nullptr);
        component->reorganizeToPackageStyle();
        _componentWidget->setComponent(component);
        _kssEditor->clear();
        return;
    }
    RulesSet ruleSet;
    RulesParser parser(QString(qApp->applicationDirPath() + "/../rules/%1.kss").arg(ruleSetName));
    QFile kssFile(QString(qApp->applicationDirPath() + "/../rules/%1.kss").arg(ruleSetName));
    kssFile.open(QIODevice::ReadOnly | QIODevice::Text);
    _kssEditor->clear();
    _kssEditor->appendPlainText(kssFile.readAll());
    parser.parse(&ruleSet);
    _kssEditor->setLineError(parser.errorLine());
    connect(_kssEditor, &KssEditor::textChanged, this, &UConfigMainWindow::updateRules);

    if (_componentWidget->component() != nullptr)
    {
        PinRuler ruler(&ruleSet);
        Component *component = _componentWidget->component();
        _componentWidget->setComponent(nullptr);
        ruler.organize(component);
        _componentWidget->setComponent(component);
        _pinListEditor->setComponent(component);
    }
}

void UConfigMainWindow::updateRules()
{
    if (_ruleComboBox->currentText() == tr("package"))
    {
        if (_componentWidget->component() != nullptr)
        {
            Component *component = _componentWidget->component();
            _componentWidget->setComponent(nullptr);
            component->reorganizeToPackageStyle();
            _componentWidget->setComponent(component);
        }
        return;
    }
    RulesSet ruleSet;
    RulesParser parser;
    parser.setData(_kssEditor->document()->toPlainText());
    _kssEditor->setLineError(parser.errorLine());
    if (!parser.parse(&ruleSet))
    {
        _kssEditor->setLineError(parser.errorLine());
        return;
    }

    _kssEditor->setLineError(0);
    PinRuler ruler(&ruleSet);
    if (_componentWidget->component() != nullptr)
    {
        Component *component = _componentWidget->component();
        _componentWidget->setComponent(nullptr);
        ruler.organize(component);
        _componentWidget->setComponent(component);
    }
}

void UConfigMainWindow::reloadRuleSetList()
{
    _ruleComboBox->clear();
    _ruleComboBox->addItem(tr("package"));
    QDir dir(qApp->applicationDirPath() + "/../rules/");
    for (const QFileInfo &ruleInfo : dir.entryInfoList(QStringList() << "*.kss", QDir::NoDotAndDotDot | QDir::Files))
    {
        _ruleComboBox->addItem(ruleInfo.baseName());
    }
}

void UConfigMainWindow::setActiveComponent(Component *component)
{
    if (component != nullptr)
    {
        _pdfDebug->setPixmap(QPixmap::fromImage(component->debugInfo()));
    }
    else
    {
        _pdfDebug->setText(tr("No pdf informations"));
    }
}

void UConfigMainWindow::createWidgets()
{
    _splitterEditor = new QSplitter(Qt::Vertical);
    _pinListEditor = new PinListEditor();
    _kssEditor = new KssEditor();
    _splitterEditor->addWidget(_pinListEditor);
    _splitterEditor->addWidget(_kssEditor);
    _splitterEditor->setSizes(QList<int>() << 200 << 200);

    QTabWidget *tabWidget = new QTabWidget();
    _componentWidget = new ComponentWidget(this);
    _componentWidget->setStyleSheet("QGraphicsView {background: white;}");
    _pdfDebug = new QLabel();
    _pdfDebug->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    _pdfDebug->setScaledContents(true);
    QScrollArea *pdfDebugArea = new QScrollArea();
    pdfDebugArea->setWidget(_pdfDebug);
    pdfDebugArea->setWidgetResizable(true);
    tabWidget->addTab(_componentWidget, tr("component"));
    tabWidget->addTab(pdfDebugArea, tr("pdf debugger"));

    _splitter = new QSplitter();
    _splitter->addWidget(_splitterEditor);
    _splitter->addWidget(tabWidget);
    _splitter->setSizes(QList<int>() << 200 << 200);

    setCentralWidget(_splitter);
    setStatusBar(new QStatusBar());

    connect(_componentWidget->viewer(), &ComponentViewer::droppedFile, _project, &UConfigProject::importComponents);

    connect(_componentWidget->viewer(), &ComponentViewer::pinsSelected, _pinListEditor->tableView(), &ComponentPinsTableView::selectPins);
    connect(_pinListEditor->tableView(), &ComponentPinsTableView::pinSelected, _componentWidget->viewer(), &ComponentViewer::selectPins);
    connect(_pinListEditor->tableView()->model(), &ComponentPinsItemModel::pinModified, _componentWidget->viewer(), &ComponentViewer::updatePin);
    connect(_pinListEditor->tableView()->model(), &ComponentPinsItemModel::pinRemoved, _componentWidget->viewer(), &ComponentViewer::removePin);
}

void UConfigMainWindow::createDocks()
{
    _componentsListDock = new QDockWidget(tr("Components list"), this);
    _componentsListDock->setObjectName("componentsList");
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
    _componentInfosDock->setObjectName("componentInfo");
    QWidget *componentInfosContent = new QWidget(_componentsListDock);
    QLayout *componentInfosLayout = new QVBoxLayout();
    componentInfosLayout->setContentsMargins(5, 5, 5, 5);
    _componentInfosEditor = new ComponentInfosEditor(_project);
    componentInfosLayout->addWidget(_componentInfosEditor);
    componentInfosContent->setLayout(componentInfosLayout);
    _componentInfosDock->setWidget(componentInfosContent);
    addDockWidget(Qt::LeftDockWidgetArea, _componentInfosDock);
}

void UConfigMainWindow::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    // MainWindow position/size/maximized
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.endGroup();
}

void UConfigMainWindow::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    // MainWindow position/size/maximized
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();

    updateOldProjects();
}

void UConfigMainWindow::updateOldProjects()
{
    for (int i = 0; i < _project->oldProjects().size(); i++)
    {
        QString path = _project->oldProjects()[i];
        _oldProjectsActions[i]->setVisible(true);
        _oldProjectsActions[i]->setData(path);
        _oldProjectsActions[i]->setText(QString("&%1. %2").arg(i + 1).arg(path));
        _oldProjectsActions[i]->setStatusTip(tr("Open recent project '") + path + "'");
    }
}

void UConfigMainWindow::createToolbarsMenus()
{
    setMenuBar(new QMenuBar(this));
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setObjectName("toolbar");
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
    connect(openFileAction,
            &QAction::triggered,
            this,
            [this]()
            {
                _project->openLib();
            });

    QAction *importFileAction = new QAction(tr("&Import components..."), this);
    importFileAction->setStatusTip(tr("Imports components from various format of files"));
    importFileAction->setShortcut(QKeySequence("Ctrl+I"));
    importFileAction->setIcon(QIcon(":/icons/img/import"));
    fileMenu->addAction(importFileAction);
    toolBar->addAction(importFileAction);
    connect(importFileAction,
            &QAction::triggered,
            this,
            [this]()
            {
                _project->importComponents();
            });

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
    connect(saveFileAsAction,
            &QAction::triggered,
            this,
            [this]()
            {
                _project->saveLibAs();
            });

    fileMenu->addSeparator();
    for (int i = 0; i < UConfigProject::MaxOldProject; i++)
    {
        QAction *recentAction = new QAction(this);
        fileMenu->addAction(recentAction);
        recentAction->setVisible(false);
        connect(recentAction, &QAction::triggered, this, &UConfigMainWindow::openRecentFile);
        _oldProjectsActions.append(recentAction);
    }

    fileMenu->addSeparator();
    QAction *exitAction = new QAction(tr("E&xit"), this);
    exitAction->setStatusTip(tr("Exits uConfig"));
    exitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // rules
    toolBar->addSeparator();
    _ruleComboBox = new QComboBox();
    connect(_ruleComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &UConfigMainWindow::organize);
    QLabel *labelFilter = new QLabel(tr(" pin ruler: "));
    toolBar->addWidget(labelFilter);
    labelFilter->setStyleSheet("QLabel {background: none}");
    QAction *actionRuleCombox = toolBar->addWidget(_ruleComboBox);
    actionRuleCombox->setStatusTip(tr("Change the rule for pin organisation"));
    QAction *ruleAction = new QAction(tr("Organize"), this);
    ruleAction->setStatusTip(tr("Organize with the current rule"));
    toolBar->addAction(ruleAction);
    connect(ruleAction, &QAction::triggered, this, &UConfigMainWindow::updateRules);

    // ============= View =============
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    viewMenu->addAction(_componentWidget->actionNoGrid());
    viewMenu->addAction(_componentWidget->actionGrid());
    viewMenu->addAction(_componentWidget->actionGridFront());
    viewMenu->addAction(_componentWidget->actionElecType());

    viewMenu->addSeparator();
    QAction *componentsListDockAction = _componentsListDock->toggleViewAction();
    viewMenu->addAction(componentsListDockAction);

    QAction *_componentInfosDockAction = _componentInfosDock->toggleViewAction();
    viewMenu->addAction(_componentInfosDockAction);

    // ============= Help =============
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction *aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, &UConfigMainWindow::about);
    helpMenu->addAction(aboutAction);

    QAction *aboutQtAction = new QAction(tr("About &Qt"), this);
    connect(aboutQtAction,
            &QAction::triggered,
            this,
            []()
            {
                QApplication::aboutQt();
            });
    helpMenu->addAction(aboutQtAction);
}

void UConfigMainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action != nullptr)
    {
        _project->openLib(action->data().toString());
    }
}

void UConfigMainWindow::about()
{
    QMessageBox::about(this, "uConfig v0", tr("Copyright (C) 2017-2020 Robotips (<a href=\"https://robotips.fr\">robotips.fr</a>)<br>\
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
Build date: ") + __DATE__ + QString(" time: ") + __TIME__);
}

bool UConfigMainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Close)
    {
        writeSettings();
        if (!_project->closeLib())
        {
            event->ignore();
            return false;
        }
    }
    return QMainWindow::event(event);
}
