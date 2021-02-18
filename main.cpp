#include "mainwindow.h"
#include "sqlite3.h"

#include <QSqlDatabase>
#include <QApplication>

int initializeDB(const char *file, sqlite3 **db, const char *os);

int main(int argc, char *argv[])
{
    const char *sim = "D:\\SIM\\sim.db"; //This should be changed to a file select box
    const char *os = "win32"; //Change this to change target system
    sqlite3 *db;

    //initializeDB(sim, &db, os); //Change this so that db initilization will create a prompt in the program

    //may possibly be a good idea to name the connections, for when multiple PCs need to access.
    QSqlDatabase qdb = QSqlDatabase::addDatabase("QSQLITE");
    qdb.setDatabaseName(sim);
    if (!qdb.open()){
        qDebug("ERROR: Qt could not make a connection to the database");
    } else {
        qDebug("Succesful Qt DB connection.");
    }
    qdb.close();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

int initializeDB(const char *file, sqlite3 **db, const char *os)
{
    //Check to see if a database already exists.
        int err = sqlite3_open_v2(file, db, SQLITE_OPEN_READONLY, os);
        if (err == 0) {
            printf("An SQLite database already exists at the location %s\nPlease delete the existing database before creating a new one.\n", file);
            sqlite3_close_v2(*db);
            return 101;
        }
        err = 0;
        sqlite3_open_v2(file, db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, os);
        sqlite3_exec(*db,
            "CREATE TABLE addresses (" // Based on xNAL standard for worldwide addresses and Drupal's webpage
                "id							INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name_line					TEXT," // Name
                "organisation_name			TEXT," // Company
                "thoroughfare				TEXT," // Street address
                "premise					TEXT," // Line 2: Apartment, Suite, Box#, Etc.;
                "sub_premise				TEXT," // Line 3: Extra
                "administrative_area		TEXT," // State / Province / Region (Use ISO Code)
                "sub_administrative_area	TEXT," // Country / District
                "locality					TEXT," // City / Town
                "dependent_locality			TEXT," // Line 2 for City
                "country					TEXT NOT NULL," // Change to 2 letter ISO Code storage before ship. May need a country table. Not too complex.
                "postal_code				TEXT," // Postal/ZIP Code
                "unsorted					TEXT"  // Used to import whole strings of addresses that have not been cleaned.
            ");",
            NULL, NULL, NULL);
        sqlite3_exec(*db,
            "CREATE TABLE shippers ("
                "id		INTEGER PRIMARY KEY AUTOINCREMENT,"
                "name	TEXT NOT NULL"
            ");",
            NULL, NULL, NULL);
        sqlite3_exec(*db,
            "CREATE TABLE suppliers ("
                "supplier_id	INTEGER PRIMARY KEY AUTOINCREMENT,"
                "supplier_name	TEXT NOT NULL,"
                "address		INTEGER,"
                "e_mail			TEXT,"
                "phone			INTEGER,"
                "phone2			INTEGER,"
                "fax			INTEGER,"
                "contact		TEXT,"
                "email			TEXT,"
                "FOREIGN KEY (address) REFERENCES addresses (id)"
            ");",
            NULL, NULL, NULL);
        sqlite3_exec(*db,
            "CREATE TABLE projects (" //Consider this as the machines that the items are used for
                "project_id		INTEGER PRIMARY KEY AUTOINCREMENT,"
                "project_name	TEXT NOT NULL"
            ");",
            NULL, NULL, NULL);
        sqlite3_exec(*db,
            "CREATE TABLE items ("
                "item_id	INTEGER PRIMARY KEY AUTOINCREMENT,"
                "item_num	TEXT NOT NULL,"
                "item_name	TEXT NOT NULL DEFAULT 'trash',"
                "item_type	TEXT,"
                "qty		INTEGER NOT NULL,"
                "price		REAL,"	//removing price for right now just to simplify (may not be useful anyway)
                "bin_num	TEXT,"	//N/A
                "shelf_num	TEXT,"	//N/a
                "replaces	TEXT,"
                "FOREIGN KEY (replaces) REFERENCES items (item_id)"
            ");",
            NULL, NULL, NULL);
        sqlite3_exec(*db,
            "CREATE TABLE po ("
                "po_num		INTEGER PRIMARY KEY AUTOINCREMENT," //Maybe UNIQUE NOT NULL in this case
                "date		DATE NOT NULL,"
                "supplier_id INTEGER REFERENCES suppliers (supplier_id),"
                "ship_to	TEXT," //Maybe make this an address reference to PHW
                "ship_by	TEXT," //Maybe make this a reference to a list of shipping handlers
                "po_status	INTEGER NOT NULL DEFAULT 3" //PO STATUS. 0 = Received PO. 1 = PO sent but not all items received PO. 2 = PO sent but no items received. 3 = Draft PO
            ");",
            NULL, NULL, NULL);
        sqlite3_exec(*db,
            "CREATE TABLE trans ("
                "trans_id		INTEGER PRIMARY KEY AUTOINCREMENT,"
                "trans_type		INTEGER NOT NULL," //0 = receiving, 1 = requesting
                "date			DATE NOT NULL,"
                "mrq_num		INTEGER,"
                "rcr_num        INTEGER,"
                "po_num			INTEGER REFERENCES po (po_num),"
                "item_id		INTEGER NOT NULL REFERENCES items (item_id),"
                "qty_change		INTEGER NOT NULL,"
                "used_for		INTEGER REFERENCES projects (project_id)," //Make not null
                "requested_by	TEXT" //REFERENCES mechanics //Make not null
            ");",
            NULL, NULL, NULL);
        sqlite3_exec(*db,
            "CREATE TABLE linker ("		//The linker is used when a single SQL column must link to multiple columns both ways.
                "link INTEGER PRIMARY KEY AUTOINCREMENT,"
                "item_id INTEGER REFERENCES items (item_id),"
                "item_qty INTEGER,"
                "item_price REAL,"
                "item_total REAL,"
                "amount_recd INTEGER,"
                "project_id INTEGER REFERENCES projects (project_id),"
                "po_num INTEGER REFERENCES po (po_num)"
            ");",
            NULL, NULL, NULL);
        /* Need to remove the linker table, since it is only used for po_details.
         * should be changed to po_details table.
         *
         * Need to add a drafts table to hold all temporary data. */
        sqlite3_exec(*db,
            "CREATE TABLE drafts ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT"
                ",type INTEGER NOT NULL /* 0: Distribute, 1: PO, 2: PR */"
                ",date DATE NOT NULL"
                ",item_id INTEGER NOT NULL REFERENCES items (item_id)"
                ",used_for INTEGER REFERENCES projects (used_for)"
                ",supplier_id INTEGER REFERENCES suppliers (supplier_id)"
                ",qty INTEGER"
                //include a field for most recent price so the program gives a price estimate
                ",current_user NOT NULL DEFAULT 0 /* 0: Debug User */"
            ");",
            NULL, NULL, NULL);
        sqlite3_exec(*db, "INSERT INTO addresses(country, unsorted) VALUES ('ZZ', 'Default Address');", NULL, NULL, NULL);
        sqlite3_close_v2(*db);

        printf("Succesfully initialized a new SQLite3 database at %s\n", file);

        return err;
}
