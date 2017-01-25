/****************************************************************************
** Meta object code from reading C++ file 'mainview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../farm_Client/farm/mainview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainView_t {
    QByteArrayData data[29];
    char stringdata0[246];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainView_t qt_meta_stringdata_MainView = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MainView"
QT_MOC_LITERAL(1, 9, 10), // "sendStatus"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 8), // "ToolType"
QT_MOC_LITERAL(4, 30, 4), // "type"
QT_MOC_LITERAL(5, 35, 5), // "Good*"
QT_MOC_LITERAL(6, 41, 4), // "good"
QT_MOC_LITERAL(7, 46, 11), // "update_data"
QT_MOC_LITERAL(8, 58, 11), // "good_change"
QT_MOC_LITERAL(9, 70, 9), // "set<Good>"
QT_MOC_LITERAL(10, 80, 5), // "goods"
QT_MOC_LITERAL(11, 86, 8), // "GoodType"
QT_MOC_LITERAL(12, 95, 11), // "connectSoil"
QT_MOC_LITERAL(13, 107, 5), // "Soil*"
QT_MOC_LITERAL(14, 113, 4), // "soil"
QT_MOC_LITERAL(15, 118, 11), // "connectTool"
QT_MOC_LITERAL(16, 130, 5), // "Tool*"
QT_MOC_LITERAL(17, 136, 4), // "tool"
QT_MOC_LITERAL(18, 141, 12), // "connectStore"
QT_MOC_LITERAL(19, 154, 6), // "Store*"
QT_MOC_LITERAL(20, 161, 5), // "store"
QT_MOC_LITERAL(21, 167, 13), // "receiveStatus"
QT_MOC_LITERAL(22, 181, 7), // "newtype"
QT_MOC_LITERAL(23, 189, 12), // "money_change"
QT_MOC_LITERAL(24, 202, 5), // "money"
QT_MOC_LITERAL(25, 208, 7), // "useGood"
QT_MOC_LITERAL(26, 216, 11), // "use_success"
QT_MOC_LITERAL(27, 228, 4), // "kind"
QT_MOC_LITERAL(28, 233, 12) // "egood_change"

    },
    "MainView\0sendStatus\0\0ToolType\0type\0"
    "Good*\0good\0update_data\0good_change\0"
    "set<Good>\0goods\0GoodType\0connectSoil\0"
    "Soil*\0soil\0connectTool\0Tool*\0tool\0"
    "connectStore\0Store*\0store\0receiveStatus\0"
    "newtype\0money_change\0money\0useGood\0"
    "use_success\0kind\0egood_change"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x06 /* Public */,
       1,    1,   79,    2, 0x26 /* Public | MethodCloned */,
       7,    1,   82,    2, 0x06 /* Public */,
       8,    2,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,   90,    2, 0x0a /* Public */,
      15,    1,   93,    2, 0x0a /* Public */,
      18,    1,   96,    2, 0x0a /* Public */,
      21,    1,   99,    2, 0x0a /* Public */,
      23,    1,  102,    2, 0x0a /* Public */,
      25,    1,  105,    2, 0x0a /* Public */,
      26,    2,  108,    2, 0x0a /* Public */,
      28,    2,  113,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11,   10,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 3,   22,
    QMetaType::Void, QMetaType::Int,   24,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,    4,   27,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 11,   10,    4,

       0        // eod
};

void MainView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainView *_t = static_cast<MainView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendStatus((*reinterpret_cast< ToolType(*)>(_a[1])),(*reinterpret_cast< Good*(*)>(_a[2]))); break;
        case 1: _t->sendStatus((*reinterpret_cast< ToolType(*)>(_a[1]))); break;
        case 2: _t->update_data((*reinterpret_cast< Good*(*)>(_a[1]))); break;
        case 3: _t->good_change((*reinterpret_cast< set<Good>(*)>(_a[1])),(*reinterpret_cast< GoodType(*)>(_a[2]))); break;
        case 4: _t->connectSoil((*reinterpret_cast< Soil*(*)>(_a[1]))); break;
        case 5: _t->connectTool((*reinterpret_cast< Tool*(*)>(_a[1]))); break;
        case 6: _t->connectStore((*reinterpret_cast< Store*(*)>(_a[1]))); break;
        case 7: _t->receiveStatus((*reinterpret_cast< ToolType(*)>(_a[1]))); break;
        case 8: _t->money_change((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->useGood((*reinterpret_cast< Good*(*)>(_a[1]))); break;
        case 10: _t->use_success((*reinterpret_cast< GoodType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->egood_change((*reinterpret_cast< set<Good>(*)>(_a[1])),(*reinterpret_cast< GoodType(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Tool* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainView::*_t)(ToolType , Good * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainView::sendStatus)) {
                *result = 0;
            }
        }
        {
            typedef void (MainView::*_t)(Good * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainView::update_data)) {
                *result = 2;
            }
        }
        {
            typedef void (MainView::*_t)(set<Good> , GoodType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainView::good_change)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject MainView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_MainView.data,
      qt_meta_data_MainView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainView.stringdata0))
        return static_cast<void*>(const_cast< MainView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int MainView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void MainView::sendStatus(ToolType _t1, Good * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void MainView::update_data(Good * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainView::good_change(set<Good> _t1, GoodType _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
