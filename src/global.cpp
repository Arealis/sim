/*
    SIM - the Simple Inventory Manager
    Copyright (C) 2021 Arealis

    This file is part of Simple Inventory Manager.

    Simple Inventory Manager is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Simple Inventory Manager is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Simple Inventory Manager.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "global.h"
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

QString returnStringINN(QString string, QString ifNotNull, QString ifNull)
{
    if (string.isEmpty())
        return ifNull;
    else
        return (ifNotNull.isNull() ? string : ifNotNull);
}

void User::clear()
{
    id.clear();
    login.clear();
    name.clear();
    email.clear();
    privelage = Viewer;
}


