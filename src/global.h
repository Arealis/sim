#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QStringList>
#include <map>
#include "advancedmacros.h"

//The following macros are incredibly convenience for prototyping enum flags, but obliterate compilation time and IDE performance. Investigate the bottleneck and avoid opening this file if possible.
#define CREATE_ENUM_STRUCT(name, type, ...)                 \
    struct name {                                           \
        enum flag : type {                                  \
            MAP(CREATE_ENUM, COMMA, 1, __VA_ARGS__)         \
        };                                                  \
        std::map<type, QString> map {                       \
            MAP(CREATE_MAPPING, COMMA, 1, __VA_ARGS__)      \
        };                                                  \
    };                                                      \
    name name

#define CREATE_MAPPING(value, key) {key, #value}

#define CREATE_ENUM(enum, value) enum = value

enum Privelage : uint8_t {Viewer, Mechanic, Purchasing, InventoryManager, Admin};
enum TableFlag : unsigned char {
    PR = 0,
    QR = 1,
    PO = 2,
    RR = 3,
    MR = 4,
};

class Status {

private:
    template <typename T, typename ... Args> T combineFlags(T flag1, Args ... moreFlags);

    template <typename T> T combineFlags(T flag);

    uint16_t m_status;

    TableFlag m_tflag;

public:
    explicit Status(int status, TableFlag tflag);

    template <typename T> T current();

    template <typename T> void clearFlags();

    template <typename T, typename ... Args> T setFlags(Args ... flags);

    template <typename T, typename ... Args> T addFlags(Args ... flags);

    template <typename T, typename ... Args> T removeFlags(Args ... flags);

    template <typename T> bool hasFlag(T flag);

    template <typename T, typename ... Args> bool hasFlags(Args ... flags);

    template <typename T> QStringList flags();

    CREATE_ENUM_STRUCT(pr,  uint16_t, Draft, Open, UserForcedUpdate, PartiallyOrdered, Ordered, PartiallyShipped, Shipped, PartiallyReceived, Received, PartiallyIssued, Issued, Rejected);
    CREATE_ENUM_STRUCT(qr,  uint8_t,  Draft, Open, UserForcedUpdate, Quoted, NotAvailable);
    CREATE_ENUM_STRUCT(po,  uint16_t, Draft, Open, UserForcedUpdate, Approved, Accepted, PartiallyShipped, Shipped, PartiallyReceived, TooMuchReceived, FullyReceived, Rejected);
    CREATE_ENUM_STRUCT(rr,  uint8_t,  Draft, Open, UserForcedUpdate, Satisfactory, Unsatisfactory, ItemsDamaged, PackagingDamaged, ItemsMissing);
    CREATE_ENUM_STRUCT(mr,  uint8_t,  Draft, Open, UserForcedUpdate, Requested, Authorized, PartiallyIssued, Issued, Rejected);

    //A UserForcedUpdate is when a document is manually changed by a user of the program. This should be used when, due to unforseen circumstances, a document must be closed
    //  before SIM is actually ready to close it. (eg. A bad supplier who never delivers on a product, or a case where product has been lost due to theft after FOB)

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

QString returnStringINN(QString string, QString ifNotNull = QString(), QString ifNull = QString());
QString escapeSql(QString string); //This will escape the (') string delimiter in SQLite to protect from SQL Injections.

#endif // GLOBAL_H
