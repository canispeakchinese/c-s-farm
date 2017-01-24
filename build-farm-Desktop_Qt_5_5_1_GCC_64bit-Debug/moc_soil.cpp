/****************************************************************************
** Meta object code from reading C++ file 'soil.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../farm_Client/farm/soil.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'soil.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Soil_t {
    QByteArrayData data[11];
    char stringdata0[79];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Soil_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Soil_t qt_meta_stringdata_Soil = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Soil"
QT_MOC_LITERAL(1, 5, 11), // "use_success"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 8), // "GoodType"
QT_MOC_LITERAL(4, 27, 4), // "type"
QT_MOC_LITERAL(5, 32, 4), // "kind"
QT_MOC_LITERAL(6, 37, 13), // "receiveStatus"
QT_MOC_LITERAL(7, 51, 8), // "ToolType"
QT_MOC_LITERAL(8, 60, 5), // "Good*"
QT_MOC_LITERAL(9, 66, 4), // "good"
QT_MOC_LITERAL(10, 71, 7) // "update2"

    },
    "Soil\0use_success\0\0GoodType\0type\0kind\0"
    "receiveStatus\0ToolType\0Good*\0good\0"
    "update2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Soil[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   34,    2, 0x0a /* Public */,
      10,    0,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 8,    4,    9,
    QMetaType::Void,

       0        // eod
};

void Soil::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Soil *_t = static_cast<Soil *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->use_success((*reinterpret_cast< GoodType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->receiveStatus((*reinterpret_cast< ToolType(*)>(_a[1])),(*reinterpret_cast< Good*(*)>(_a[2]))); break;
        case 2: _t->update2(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Soil::*_t)(GoodType , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Soil::use_success)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject Soil::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Soil.data,
      qt_meta_data_Soil,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Soil::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Soil::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Soil.stringdata0))
        return static_cast<void*>(const_cast< Soil*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< Soil*>(this));
    return QObject::qt_metacast(_clname);
}

int Soil::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void Soil::use_success(GoodType _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_SoilGroup_t {
    QByteArrayData data[5];
    char stringdata0[31];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SoilGroup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SoilGroup_t qt_meta_stringdata_SoilGroup = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SoilGroup"
QT_MOC_LITERAL(1, 10, 8), // "sendSoil"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 5), // "Soil*"
QT_MOC_LITERAL(4, 26, 4) // "soil"

    },
    "SoilGroup\0sendSoil\0\0Soil*\0soil"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SoilGroup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void SoilGroup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SoilGroup *_t = static_cast<SoilGroup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendSoil((*reinterpret_cast< Soil*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Soil* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (SoilGroup::*_t)(Soil * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&SoilGroup::sendSoil)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject SoilGroup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SoilGroup.data,
      qt_meta_data_SoilGroup,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SoilGroup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SoilGroup::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SoilGroup.stringdata0))
        return static_cast<void*>(const_cast< SoilGroup*>(this));
    if (!strcmp(_clname, "QGraphicsItemGroup"))
        return static_cast< QGraphicsItemGroup*>(const_cast< SoilGroup*>(this));
    return QObject::qt_metacast(_clname);
}

int SoilGroup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void SoilGroup::sendSoil(Soil * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
