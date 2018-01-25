#include <QDebug>
#include <QApplication>

#include "uconfigmainwindow.h"
#include "project/uconfigproject.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    UConfigProject project;
    UConfigMainWindow mainWindow(&project);
    project.setWindow(&mainWindow);
    mainWindow.show();
    if (app.arguments().size() > 1)
    {
        QString fileArg = app.arguments()[1];
        if (fileArg.endsWith(".lib"))
            project.openLib(fileArg);
        else
            project.importComponents(fileArg);
    }
    else
        project.newLib();

    return app.exec();
}
