/****************************************************************************
** Meta object code from reading C++ file 'showscenegroup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../farm_Client/farm/showscenegroup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'showscenegroup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ShowSceneGroup_t {
    QByteArrayData data[8];
    char stringdata0[59];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ShowSceneGroup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ShowSceneGroup_t qt_meta_stringdata_ShowSceneGroup = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ShowSceneGroup"
QT_MOC_LITERAL(1, 15, 9), // "sendStore"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 6), // "Store*"
QT_MOC_LITERAL(4, 33, 5), // "store"
QT_MOC_LITERAL(5, 39, 8), // "sendShop"
QT_MOC_LITERAL(6, 48, 5), // "Shop*"
QT_MOC_LITERAL(7, 54, 4) // "shop"

    },
    "ShowSceneGroup\0sendStore\0\0Store*\0store\0"
    "sendShop\0Shop*\0shop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ShowSceneGroup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       5,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void ShowSceneGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ShowSceneGroup *_t = static_cast<ShowSceneGroup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendStore((*reinterpret_cast< Store*(*)>(_a[1]))); break;
        case 1: _t->sendShop((*reinterpret_cast< Shop*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ShowSceneGroup::*_t)(Store * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowSceneGroup::sendStore)) {
                *result = 0;
            }
        }
        {
            typedef void (ShowSceneGroup::*_t)(Shop * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ShowSceneGroup::sendShop)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ShowSceneGroup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ShowSceneGroup.data,
      qt_meta_data_ShowSceneGroup,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ShowSceneGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ShowSceneGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ShowSceneGroup.stringdata0))
        return static_cast<void*>(const_cast< ShowSceneGroup*>(this));
    if (!strcmp(_clname, "QGraphicsItemGroup"))
        return static_cast< QGraphicsItemGroup*>(const_cast< ShowSceneGroup*>(this));
    return QObject::qt_metacast(_clname);
}

int ShowSceneGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ShowSceneGroup::sendStore(Store * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ShowSceneGroup::sendShop(Shop * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
