#include "uconfigmainwindow.h"

#include "importer/pinlistimporter.h"

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

UConfigMainWindow::UConfigMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/icons/img/uConfig.ico"));
    createWidgets();
    createDocks();
    createToolbarsMenus();
    setWindowTitle("uConfig");
    reloadRuleSetList();
    setAcceptDrops(true);

    resize(QApplication::primaryScreen()->size()*.7);

#if 0
    Datasheet datasheet;
    //datasheet.open("../../../projects/DataSheets/mem/IS61WV25616BLL.pdf");
    datasheet.open("../../../projects/DataSheets/Microchip/PIC32/PIC32MM_GPM_revC.pdf");
    datasheet.setDebugEnabled(true);
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
        importComponents(fileName);
    }
}

void UConfigMainWindow::importComponents(const QString &fileName)
{
    PinListImporter importer(fileName, this);
    importer.exec();

    foreach (Component *component, importer.components())
    {
        _componentsTreeView->addComponent(component);
    }
    if (!_componentsTreeView->lib()->components().empty())
    {
        selectComponent(_componentsTreeView->lib()->components()[0]);
    }
    _componentsTreeView->resizeColumnToContents(0);
    _componentsTreeView->resizeColumnToContents(1);

    _importedPathLib = importer.filePath();
}

void UConfigMainWindow::saveLib()
{
    saveLibAs(_pathLib);
}

void UConfigMainWindow::saveLibAs(const QString &fileName)
{
    QString libFileName;

    if (fileName.isEmpty())
    {
        QFileDialog fileDialog(this);
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        fileDialog.setDefaultSuffix(".lib");
        fileDialog.setNameFilter(tr("Kicad component library (*.lib)"));
        fileDialog.setWindowTitle(tr("Save Kicad library"));
        if (!_importedPathLib.isEmpty())
        {
            libFileName = _importedPathLib;
            libFileName.replace(QRegExp("(.*)\\.(pdf|csv)"), "\\1.lib");
            fileDialog.selectFile(libFileName);
        }
        if (fileDialog.exec())
            libFileName = fileDialog.selectedFiles().first();
        if (libFileName.isEmpty())
            return;
    }
    else
        libFileName = fileName;

    if (!libFileName.endsWith(".lib"))
        libFileName.append(".lib");
    _pathLib = libFileName;

    _componentsTreeView->lib()->saveTo(libFileName);
}

void UConfigMainWindow::selectComponent(Component *component)
{
    _componentsPinTableView->setComponent(component);
    _componentViewer->setComponent(component);

    if (!component)
        return;

    organize(_ruleComboBox->currentText());
    if (!component->debugInfo().isNull())
    {
        QImage image = component->debugInfo();
        image = image.scaledToHeight(QApplication::primaryScreen()->size().height());
        _pdfDebug->setPixmap(QPixmap::fromImage(image));
    }
}

void UConfigMainWindow::organize(QString ruleSetName)
{
    disconnect(_kssEditor, &KssEditor::textChanged, this, &UConfigMainWindow::updateRules);
    if (ruleSetName == "package")
    {
        if (!_componentViewer->component())
            return;
        Component *component = _componentViewer->component();
        _componentViewer->scene()->clear();
        component->reorganizeToPackageStyle();
        _componentViewer->setComponent(component);
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

    if (_componentViewer->component())
    {
        PinRuler ruler(&ruleSet);
        Component *component = _componentViewer->component();
        _componentViewer->scene()->clear();
        ruler.organize(component);
        _componentViewer->setComponent(component);
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
    if (_componentViewer->component())
    {
        Component *component = _componentViewer->component();
        _componentViewer->scene()->clear();
        ruler.organize(component);
        _componentViewer->setComponent(component);
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
    _componentsPinTableView = new ComponentPinsTableView();
    _kssEditor = new KssEditor();
    _splitterEditor->addWidget(_componentsPinTableView);
    _splitterEditor->addWidget(_kssEditor);
    _splitterEditor->setSizes(QList<int>()<<200<<200);

    QTabWidget *tabWidget = new QTabWidget();
    _componentViewer = new ComponentViewer();
    _pdfDebug = new QLabel();
    _pdfDebug->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
    _pdfDebug->setScaledContents(true);
    QScrollArea *pdfDebugArea = new QScrollArea();
    pdfDebugArea->setWidget(_pdfDebug);
    pdfDebugArea->setWidgetResizable(true);
    tabWidget->addTab(_componentViewer, "component");
    tabWidget->addTab(pdfDebugArea, "pdf debugger");

    _splitter = new QSplitter();
    _splitter->addWidget(_splitterEditor);
    _splitter->addWidget(tabWidget);
    _splitter->setSizes(QList<int>()<<200<<200);

    setCentralWidget(_splitter);
    setStatusBar(new QStatusBar());

    connect(_componentViewer, &ComponentViewer::droppedFile,
            this, &UConfigMainWindow::importComponents);

    connect(_componentViewer, &ComponentViewer::pinSelected,
            _componentsPinTableView, &ComponentPinsTableView::selectPin);
    connect(_componentsPinTableView, &ComponentPinsTableView::pinSelected,
            _componentViewer, &ComponentViewer::selectPin);
    connect(_componentsPinTableView->model(), &ComponentPinsItemModel::pinModified,
            _componentViewer, &ComponentViewer::updatePin);
}

void UConfigMainWindow::createDocks()
{
    _componentsListDock = new QDockWidget(tr("Components list"), this);
    QWidget *componentsListContent = new QWidget(_componentsListDock);
    QLayout *componentsListLayout = new QVBoxLayout();
    componentsListLayout->setContentsMargins(5, 5, 5, 5);
    _componentsTreeView = new ComponentLibTreeView();
    _componentsTreeView->setEditMode(true);
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
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(importComponents()));

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

    // rules
    toolBar->addSeparator();
    _ruleComboBox = new QComboBox();
    connect(_ruleComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(organize(QString)));
    toolBar->addWidget(new QLabel(tr(" pin ruler: ")));
    QAction *actionRuleCombox = toolBar->addWidget(_ruleComboBox);
    actionRuleCombox->setStatusTip(tr("Change the rule for pin organisation"));

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
