#include <QDebug>
#include <QApplication>

#include "uconfigmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    UConfigMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
