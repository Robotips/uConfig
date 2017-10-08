#ifndef UCONFIGMAINWINDOW_H
#define UCONFIGMAINWINDOW_H

#include <QMainWindow>

#include <QTreeView>
#include <QTableView>
#include <QSplitter>
#include <QDockWidget>

#include "viewer/componentviewer.h"
#include "itemmodel/componentlibtreeview.h"
#include "itemmodel/componentpinstableview.h"

class UConfigMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit UConfigMainWindow(QWidget *parent = nullptr);

protected slots:
    void importComponents();
    void saveLib();
    void saveLibAs(const QString &fileName = QString());

    void selectComponent(Component *component);

    void about();
protected:
    void createWidgets();
    QSplitter *_splitter;
    ComponentLibTreeView *_componentsTreeView;
    ComponentPinsTableView *_componentsPinTableView;
    ComponentViewer *_componentViewer;

    void createDocks();
    QDockWidget *_componentsListDock;

    void createToolbarsMenus();

    QString _pathLib;
};

#endif // UCONFIGMAINWINDOW_H
