#include <QDebug>
#include <QApplication>

#include "uconfigmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    UConfigMainWindow mainWindow;
    mainWindow.show();
    if (app.arguments().size() > 1)
        mainWindow.importComponents(app.arguments()[1]);

    return app.exec();
}
