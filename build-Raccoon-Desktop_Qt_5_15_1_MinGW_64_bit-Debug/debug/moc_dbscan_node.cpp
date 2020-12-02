/****************************************************************************
** Meta object code from reading C++ file 'dbscan_node.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Raccoon/Includes/gui/dbscan_node.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dbscan_node.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_dbscan_node_t {
    QByteArrayData data[9];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dbscan_node_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dbscan_node_t qt_meta_stringdata_dbscan_node = {
    {
QT_MOC_LITERAL(0, 0, 11), // "dbscan_node"
QT_MOC_LITERAL(1, 12, 9), // "preview_b"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 13), // "metric_change"
QT_MOC_LITERAL(4, 37, 3), // "str"
QT_MOC_LITERAL(5, 41, 10), // "eps_change"
QT_MOC_LITERAL(6, 52, 3), // "eps"
QT_MOC_LITERAL(7, 56, 10), // "num_change"
QT_MOC_LITERAL(8, 67, 1) // "n"

    },
    "dbscan_node\0preview_b\0\0metric_change\0"
    "str\0eps_change\0eps\0num_change\0n"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dbscan_node[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    1,   35,    2, 0x0a /* Public */,
       5,    1,   38,    2, 0x0a /* Public */,
       7,    1,   41,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Int,    8,

       0        // eod
};

void dbscan_node::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<dbscan_node *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->preview_b(); break;
        case 1: _t->metric_change((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->eps_change((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->num_change((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject dbscan_node::staticMetaObject = { {
    QMetaObject::SuperData::link<node::staticMetaObject>(),
    qt_meta_stringdata_dbscan_node.data,
    qt_meta_data_dbscan_node,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *dbscan_node::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dbscan_node::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_dbscan_node.stringdata0))
        return static_cast<void*>(this);
    return node::qt_metacast(_clname);
}

int dbscan_node::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = node::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
