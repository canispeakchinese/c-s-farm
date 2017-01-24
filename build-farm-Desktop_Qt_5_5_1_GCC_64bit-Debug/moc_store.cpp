/****************************************************************************
** Meta object code from reading C++ file 'store.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../farm_Client/farm/store.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'store.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Store_t {
    QByteArrayData data[15];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Store_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Store_t qt_meta_stringdata_Store = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Store"
QT_MOC_LITERAL(1, 6, 12), // "money_change"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 11), // "update_data"
QT_MOC_LITERAL(4, 32, 5), // "money"
QT_MOC_LITERAL(5, 38, 11), // "good_change"
QT_MOC_LITERAL(6, 50, 9), // "set<Good>"
QT_MOC_LITERAL(7, 60, 4), // "good"
QT_MOC_LITERAL(8, 65, 8), // "GoodType"
QT_MOC_LITERAL(9, 74, 4), // "type"
QT_MOC_LITERAL(10, 79, 14), // "business_judge"
QT_MOC_LITERAL(11, 94, 4), // "Good"
QT_MOC_LITERAL(12, 99, 3), // "num"
QT_MOC_LITERAL(13, 103, 11), // "buy_success"
QT_MOC_LITERAL(14, 115, 5) // "Good*"

    },
    "Store\0money_change\0\0update_data\0money\0"
    "good_change\0set<Good>\0good\0GoodType\0"
    "type\0business_judge\0Good\0num\0buy_success\0"
    "Good*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Store[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    1,   47,    2, 0x06 /* Public */,
       5,    2,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    2,   55,    2, 0x0a /* Public */,
      13,    2,   60,    2, 0x0a /* Public */,
       3,    1,   65,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8,    7,    9,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,    7,   12,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,    7,   12,
    QMetaType::Void, 0x80000000 | 14,    7,

       0        // eod
};

void Store::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Store *_t = static_cast<Store *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->money_change((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->update_data((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->good_change((*reinterpret_cast< set<Good>(*)>(_a[1])),(*reinterpret_cast< GoodType(*)>(_a[2]))); break;
        case 3: _t->business_judge((*reinterpret_cast< Good(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->buy_success((*reinterpret_cast< Good(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->update_data((*reinterpret_cast< Good*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Store::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Store::money_change)) {
                *result = 0;
            }
        }
        {
            typedef void (Store::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Store::update_data)) {
                *result = 1;
            }
        }
        {
            typedef void (Store::*_t)(set<Good> , GoodType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Store::good_change)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject Store::staticMetaObject = {
    { &WareHouse::staticMetaObject, qt_meta_stringdata_Store.data,
      qt_meta_data_Store,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Store::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Store::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Store.stringdata0))
        return static_cast<void*>(const_cast< Store*>(this));
    return WareHouse::qt_metacast(_clname);
}

int Store::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = WareHouse::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Store::money_change(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Store::update_data(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Store::good_change(set<Good> _t1, GoodType _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
