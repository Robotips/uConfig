#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);

    download = new DatasheetDownloader();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open(QString file)
{
    /*PicFile pic;
    qDebug()<<file;
    pic.open(file);*/
    data.open(file);
    data.analyse();
    ui->listWidget->clear();
    foreach (DatasheetPackage *package, data.packages())
    {
        ui->listWidget->addItem(package->name);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    event->accept();
    open(event->mimeData()->urls().first().path().mid(1));
}

void MainWindow::on_pushButton_clicked()
{
    /*QSqlQuery querry;
    querry.exec(QString("SELECT DISTINCT dsid FROM UPROC"));
    while(querry.next())
    {
        int dsid=querry.value(0).toInt();
        download->download(dsid);
    }*/

    /*Datasheet data;
    data.open(61156);
    //data.open("edc/doc/61156g.pdf");
    data.pinSearch(data.pagePinDiagram());*/

    //db->updateDatabase();
}

void MainWindow::on_listWidget_activated(const QModelIndex &index)
{
    DatasheetPackage *package = data.packages().at(index.row());

    ui->textBrowser->clear();
    ui->label->setPixmap(QPixmap::fromImage(package->image));
    foreach (DatasheetPin *pin, package->pins)
    {
        ui->textBrowser->append(QString("%1\t%2").arg(pin->pin).arg(pin->name));
    }
}
