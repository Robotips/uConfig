#ifndef UCONFIGMAINWINDOW_H
#define UCONFIGMAINWINDOW_H

#include <QMainWindow>

#include <QTreeView>
#include <QTableView>
#include <QSplitter>
#include <QDockWidget>
#include <QComboBox>
#include <QLabel>

#include "project/uconfigproject.h"

#include "viewer/componentviewer.h"
#include "itemmodel/componentlibtreeview.h"
#include "itemmodel/pinlisteditor.h"
#include "ksseditor/ksseditor.h"
#include "componentinfoseditor.h"

class UConfigMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    UConfigMainWindow(UConfigProject *project);
    ~UConfigMainWindow();

protected slots:
    void setTitle();

    void organize(QString ruleSetName);
    void updateRules();
    void reloadRuleSetList();

    void about();

protected:
    UConfigProject *_project;

    void createWidgets();
    QSplitter *_splitter;
    ComponentLibTreeView *_componentsTreeView;
    ComponentInfosEditor *_componentInfosEditor;
    PinListEditor *_pinListEditor;
    ComponentViewer *_componentViewer;
    QComboBox *_ruleComboBox;
    QLabel *_pdfDebug;

    QSplitter *_splitterEditor;
    KssEditor *_kssEditor;

    void createDocks();
    QDockWidget *_componentsListDock;
    QDockWidget *_componentInfosDock;

    void createToolbarsMenus();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // UCONFIGMAINWINDOW_H
