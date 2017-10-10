#ifndef UCONFIGMAINWINDOW_H
#define UCONFIGMAINWINDOW_H

#include <QMainWindow>

#include <QTreeView>
#include <QTableView>
#include <QSplitter>
#include <QDockWidget>
#include <QComboBox>
#include <QLabel>

#include "viewer/componentviewer.h"
#include "itemmodel/componentlibtreeview.h"
#include "itemmodel/componentpinstableview.h"

class UConfigMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    UConfigMainWindow(QWidget *parent = Q_NULLPTR);

protected slots:
    void importComponents(const QString &fileName = QString());
    void saveLib();
    void saveLibAs(const QString &fileName = QString());

    void selectComponent(Component *component);

    void organize(QString ruleSetName);
    void reloadRuleSetList();

    void about();

protected:
    void createWidgets();
    QSplitter *_splitter;
    ComponentLibTreeView *_componentsTreeView;
    ComponentPinsTableView *_componentsPinTableView;
    ComponentViewer *_componentViewer;
    QComboBox *_ruleComboBox;
    QLabel *_pdfDebug;

    void createDocks();
    QDockWidget *_componentsListDock;

    void createToolbarsMenus();

    QString _pathLib;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // UCONFIGMAINWINDOW_H
