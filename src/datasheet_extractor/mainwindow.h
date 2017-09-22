#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "udatabase.h"
#include "datasheetdownloader.h"
#include "datasheet.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void open(QString file);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_listWidget_activated(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    UDataBase *db;
    DatasheetDownloader *download;
    Datasheet data;
};

#endif  // MAINWINDOW_H
