#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <map>


//enum TableFlag : int {PR, QR, PO, RR, MR};

enum Privelage : int {Viewer, Mechanic, Purchasing, InventoryManager, Admin};
enum TableFlag : unsigned char {
    InventoryHistory,
    Items, ItemDetails,
    Projects, ProjectDetails,
    Suppliers, SupplierDetails,
    PR, PRD, QR, QRD, PO, POD, RR, RRD, MR, MRD,
};

struct User {
    QString id; //Is a QString instead of an int for convenience when inserting into sql statements
    QString login;
    QString name;
    QString email;
    Privelage privelage;
    void clear();
};

namespace CSS {
    static const QString Normal = "color: #000000; background-color: #ffffff;"; //grey
    static const QString Alert = "color: #aa4471; background-color: #ffe4e4;"; //red
};

QString returnStringINN(QString string, QString ifNotNull, QString ifNull = "");
QString escapeSql(QString string); //This will escape the (') string delimiter in SQLite to protect from SQL Injections.

#endif // GLOBAL_H
