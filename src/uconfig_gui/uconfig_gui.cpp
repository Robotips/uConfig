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
        project.importComponents(app.arguments()[1]);
    else
        project.newLib();

    return app.exec();
}
