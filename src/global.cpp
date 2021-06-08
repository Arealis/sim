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

#include <QStringBuilder>
#include <QRegularExpression>
#include "global.h"

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

//Templates for flags

Status::Status(int status, TableFlag tflag)
{
    m_status = status;
    m_tflag = tflag;
}

template <typename T>
QStringList Status::flags() //This function is functional but terrible, and signifies a massive problem with how enums are handled right now.
{
    T it, end;
    switch (m_tflag) {
    case PR: {
        it = pr.map.begin();
        end = pr.map.end();
        break;
    }
    case QR: {
        it = qr.map.begin();
        end = qr.map.end();
        break;
    }
    case PO: {
        it = po.map.begin();
        end = po.map.end();
        break;
    }
    case RR: {
        it = rr.map.begin();
        end = rr.map.end();
        break;
    }
    case MR: {
        it = mr.map.begin();
        end = mr.map.end();
        break;
    }
    }

    QStringList flags;
    while (it != end) {
        if (hasFlag(it->first)) {
            flags.append(it->second);
        }
    }
    return flags;
}

template <typename T>
T Status::current()
{
    return m_status;
}

template <typename T, typename ... Args>
T Status::setFlags(Args ... flags)
{
    return m_status = combineFlags(flags...);
}

template <typename T, typename ... Args>
T Status::addFlags(Args ... flags)
{
    return m_status = m_status | combineFlags(flags...);
}

template <typename T, typename ... Args>
T Status::removeFlags(Args ... flags)
{
    return m_status = m_status & (~combineFlags(flags...));
}

template <typename T>
bool Status::hasFlag(T flag)
{
    if ((flag & m_status) != 0)
        return true;
    else
        return false;
}

template <typename T, typename ... Args>
bool Status::hasFlags(Args ... flags)
{
    T flag = combineFlags(flags...);
    if ((flag & ~(m_status & flag)) == 0)
        return true;
    else
        return false;
}

template <typename T, typename ... Args>
T Status::combineFlags(T flag1, Args ... moreFlags)
{
    return flag1 | combineFlags(moreFlags...);
}

template <typename T>
T Status::combineFlags(T flag1)
{
    return flag1;
}


