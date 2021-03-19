#ifndef GLOBAL_H
#define GLOBAL_H

#include <QStringBuilder>
#include <QRegularExpression>


QString escapeSql(QString string) //This will escape the (') string delimiter in SQLite to protect from SQL Injections.
{
    if (string.isEmpty())
        return "null";

    QRegularExpression re(QRegularExpression::anchoredPattern("\\d*"));
    QRegularExpression re2(QRegularExpression::anchoredPattern("\\d+\\.\\d*"));
    if (re.match(string).hasMatch() || re2.match(string).hasMatch())
        return string;

    for (int i = string.indexOf("'"); i != -1; i = string.indexOf("'", i+2))
        string.insert(i, "'");
    return "'"%string%"'";
}

#endif // GLOBAL_H
