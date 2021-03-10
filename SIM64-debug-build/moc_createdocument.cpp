/****************************************************************************
** Meta object code from reading C++ file 'createdocument.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.0.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/createdocument.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'createdocument.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.0.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CreateDocument_t {
    const uint offsetsAndSize[36];
    char stringdata0[220];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_CreateDocument_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_CreateDocument_t qt_meta_stringdata_CreateDocument = {
    {
QT_MOC_LITERAL(0, 14), // "CreateDocument"
QT_MOC_LITERAL(15, 20), // "on_addCustom_clicked"
QT_MOC_LITERAL(36, 0), // ""
QT_MOC_LITERAL(37, 15), // "on_save_clicked"
QT_MOC_LITERAL(53, 17), // "on_cancel_clicked"
QT_MOC_LITERAL(71, 20), // "on_saveDraft_clicked"
QT_MOC_LITERAL(92, 10), // "storeTable"
QT_MOC_LITERAL(103, 9), // "tableFlag"
QT_MOC_LITERAL(113, 9), // "oldDocNum"
QT_MOC_LITERAL(123, 9), // "newDocNum"
QT_MOC_LITERAL(133, 27), // "insertRecurringCustomFields"
QT_MOC_LITERAL(161, 12), // "QSqlDatabase"
QT_MOC_LITERAL(174, 2), // "db"
QT_MOC_LITERAL(177, 4), // "int*"
QT_MOC_LITERAL(182, 3), // "row"
QT_MOC_LITERAL(186, 13), // "onCellChanged"
QT_MOC_LITERAL(200, 6), // "column"
QT_MOC_LITERAL(207, 12) // "deleteCustom"

    },
    "CreateDocument\0on_addCustom_clicked\0"
    "\0on_save_clicked\0on_cancel_clicked\0"
    "on_saveDraft_clicked\0storeTable\0"
    "tableFlag\0oldDocNum\0newDocNum\0"
    "insertRecurringCustomFields\0QSqlDatabase\0"
    "db\0int*\0row\0onCellChanged\0column\0"
    "deleteCustom"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CreateDocument[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x08,    0 /* Private */,
       3,    0,   63,    2, 0x08,    1 /* Private */,
       4,    0,   64,    2, 0x08,    2 /* Private */,
       5,    0,   65,    2, 0x08,    3 /* Private */,
       6,    3,   66,    2, 0x08,    4 /* Private */,
      10,    3,   73,    2, 0x08,    8 /* Private */,
      15,    2,   80,    2, 0x08,   12 /* Private */,
      17,    0,   85,    2, 0x08,   15 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString,    7,    8,    9,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QString, 0x80000000 | 13,   12,    7,   14,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   14,   16,
    QMetaType::Void,

       0        // eod
};

void CreateDocument::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CreateDocument *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_addCustom_clicked(); break;
        case 1: _t->on_save_clicked(); break;
        case 2: _t->on_cancel_clicked(); break;
        case 3: _t->on_saveDraft_clicked(); break;
        case 4: _t->storeTable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 5: _t->insertRecurringCustomFields((*reinterpret_cast< QSqlDatabase(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int*(*)>(_a[3]))); break;
        case 6: _t->onCellChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->deleteCustom(); break;
        default: ;
        }
    }
}

const QMetaObject CreateDocument::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CreateDocument.offsetsAndSize,
    qt_meta_data_CreateDocument,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_CreateDocument_t

, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QSqlDatabase, std::false_type>, QtPrivate::TypeAndForceComplete<QString, std::false_type>, QtPrivate::TypeAndForceComplete<int *, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *CreateDocument::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CreateDocument::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CreateDocument.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int CreateDocument::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}
struct qt_meta_stringdata_ResizableTable_t {
    const uint offsetsAndSize[2];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ResizableTable_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ResizableTable_t qt_meta_stringdata_ResizableTable = {
    {
QT_MOC_LITERAL(0, 14) // "ResizableTable"

    },
    "ResizableTable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ResizableTable[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ResizableTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    (void)_o;
    (void)_id;
    (void)_c;
    (void)_a;
}

const QMetaObject ResizableTable::staticMetaObject = { {
    QMetaObject::SuperData::link<QTableWidget::staticMetaObject>(),
    qt_meta_stringdata_ResizableTable.offsetsAndSize,
    qt_meta_data_ResizableTable,
    qt_static_metacall,
    nullptr,
    nullptr,
    nullptr
} };


const QMetaObject *ResizableTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ResizableTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ResizableTable.stringdata0))
        return static_cast<void*>(this);
    return QTableWidget::qt_metacast(_clname);
}

int ResizableTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
