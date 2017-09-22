#include "udatabase.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QDebug>

#include "picfile.h"

UDataBase::UDataBase()
{
    _db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    _db->setDatabaseName("uproc.db");
    _db->open();

    createDB();
}

UDataBase::~UDataBase()
{
    _db->close();
}

int UDataBase::idFromName(QString name)
{
    QSqlQuery querry;
    querry.exec(QString("SELECT ID FROM UPROC WHERE NAME='%1'").arg(name));
    if (querry.next())
        return querry.value(0).toInt();
    else
        return -1;
}

void UDataBase::update(int id, QString fieldName, QVariant value)
{
    QSqlQuery querry;
    if (!querry.exec(QString("UPDATE UPROC set %2=%3 WHERE ID=%1")
                         .arg(id)
                         .arg(fieldName)
                         .arg(value.toString())))
        qDebug() << querry.lastError() << "  :  " << querry.lastQuery();
}

void UDataBase::insertField(QString fieldName)
{
    QSqlQuery querry;
    querry.exec(QString("ALTER TABLE UPROC ADD %1 int").arg(fieldName));
    if (!querry.next())
        qDebug() << querry.lastError();
}

void UDataBase::updateDatabase()
{
    PicFile pic;
    QSqlQuery querry;
    querry.exec(QString("SELECT NAME,ID FROM UPROC"));
    while (querry.next())
    {
        QString name = querry.value(0).toString();
        int id = querry.value(1).toInt();
        if (pic.open("edc/pic/" + name + ".pic"))
        {
            update(id, "psid", QVariant(pic.psid()));
            update(id, "dsid", QVariant(pic.dsid()));
        }
    }
}

void UDataBase::createDB()
{
    QSqlQuery querry;
    QDir dir;
    querry.exec("PRAGMA synchronous = OFF;");

    // insertField("dsid");

    // querry.exec("CREATE TABLE UPROC (name varchar(50), id integer primary
    // key);");

    /*QStringList filters;
    filters << "*.pic";
    dir.setNameFilters(filters);

    dir.setPath(QDir::currentPath()+"/edc/pic");
    int i = 0;
    foreach(QString file, dir.entryList())
    {
        if(idFromName(file.section('.',0,0))==-1) querry.exec(QString("INSERT
    INTO UPROC VALUES('%1', NULL, NULL, NULL);").arg(file.section('.',0,0)));
    }*/
}
