#ifndef UDATABASE_H
#define UDATABASE_H

#include <QSqlDatabase>
#include <QVariant>

class UDataBase
{
public:
    UDataBase();
    ~UDataBase();

    int idFromName(QString name);
    void update(int id, QString fieldName, QVariant value);
    void insertField(QString fieldName);
    void updateDatabase();

private:
    QSqlDatabase *_db;

    void createDB();
};

#endif  // UDATABASE_H
