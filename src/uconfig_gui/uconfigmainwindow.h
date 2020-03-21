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

    void organize(QString ruleSetName);
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

#endif // UCONFIGMAINWINDOW_H
