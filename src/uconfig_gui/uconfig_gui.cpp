#include <QApplication>
#include <QDebug>

#include "project/uconfigproject.h"
#include "uconfigmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Robotips");
    app.setOrganizationDomain("Robotips");
    app.setApplicationName("uConfig");

    UConfigProject project;
    UConfigMainWindow mainWindow(&project);
    project.setWindow(&mainWindow);
    mainWindow.show();
    if (app.arguments().size() > 1)
    {
        QString fileArg = app.arguments()[1];
        if (fileArg.endsWith(".lib", Qt::CaseInsensitive))
        {
            project.openLib(fileArg);
        }
        else
        {
            project.importComponents(fileArg);
        }
    }
    else
    {
        project.newLib();
    }

    return app.exec();
}
