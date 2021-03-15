#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

QString escapeSql(QString string) //This will escape the (') string delimiter in SQLite to protect from SQL Injections.
{
    for (int i = string.indexOf("'"); i != -1; i = string.indexOf("'", i+2))
        string.insert(i, "'");
    return string;
}
#endif // GLOBAL_H
