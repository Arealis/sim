/****************************************************************************
** Meta object code from reading C++ file 'receiveitems.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.0.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/receiveitems.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receiveitems.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.0.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ReceiveItems_t {
    const uint offsetsAndSize[28];
    char stringdata0[290];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_ReceiveItems_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_ReceiveItems_t qt_meta_stringdata_ReceiveItems = {
    {
QT_MOC_LITERAL(0, 12), // "ReceiveItems"
QT_MOC_LITERAL(13, 18), // "on_Discard_clicked"
QT_MOC_LITERAL(32, 0), // ""
QT_MOC_LITERAL(33, 18), // "on_Confirm_clicked"
QT_MOC_LITERAL(52, 25), // "on_ItemIdText_textChanged"
QT_MOC_LITERAL(78, 4), // "arg1"
QT_MOC_LITERAL(83, 29), // "on_ItemIdText_editingFinished"
QT_MOC_LITERAL(113, 27), // "on_ItemIdText_returnPressed"
QT_MOC_LITERAL(141, 21), // "onItemIdTextCompleted"
QT_MOC_LITERAL(163, 33), // "on_POComboBox_currentIndexCha..."
QT_MOC_LITERAL(197, 5), // "index"
QT_MOC_LITERAL(203, 36), // "on_ItemIdComboBox_currentText..."
QT_MOC_LITERAL(240, 26), // "on_UsedForText_textChanged"
QT_MOC_LITERAL(267, 22) // "onUsedForTextCompleted"

    },
    "ReceiveItems\0on_Discard_clicked\0\0"
    "on_Confirm_clicked\0on_ItemIdText_textChanged\0"
    "arg1\0on_ItemIdText_editingFinished\0"
    "on_ItemIdText_returnPressed\0"
    "onItemIdTextCompleted\0"
    "on_POComboBox_currentIndexChanged\0"
    "index\0on_ItemIdComboBox_currentTextChanged\0"
    "on_UsedForText_textChanged\0"
    "onUsedForTextCompleted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ReceiveItems[] = {

 // content:
       9,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x08,    0 /* Private */,
       3,    0,   75,    2, 0x08,    1 /* Private */,
       4,    1,   76,    2, 0x08,    2 /* Private */,
       6,    0,   79,    2, 0x08,    4 /* Private */,
       7,    0,   80,    2, 0x08,    5 /* Private */,
       8,    0,   81,    2, 0x08,    6 /* Private */,
       9,    1,   82,    2, 0x08,    7 /* Private */,
      11,    1,   85,    2, 0x08,    9 /* Private */,
      12,    1,   88,    2, 0x08,   11 /* Private */,
      13,    0,   91,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

       0        // eod
};

void ReceiveItems::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ReceiveItems *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_Discard_clicked(); break;
        case 1: _t->on_Confirm_clicked(); break;
        case 2: _t->on_ItemIdText_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_ItemIdText_editingFinished(); break;
        case 4: _t->on_ItemIdText_returnPressed(); break;
        case 5: _t->onItemIdTextCompleted(); break;
        case 6: _t->on_POComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_ItemIdComboBox_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->on_UsedForText_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->onUsedForTextCompleted(); break;
        default: ;
        }
    }
}

const QMetaObject ReceiveItems::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ReceiveItems.offsetsAndSize,
    qt_meta_data_ReceiveItems,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_ReceiveItems_t

, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<const QString &, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *ReceiveItems::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReceiveItems::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ReceiveItems.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ReceiveItems::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
