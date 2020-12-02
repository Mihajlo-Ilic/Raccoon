/****************************************************************************
** Meta object code from reading C++ file 'k_mean_cluster_node.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Raccoon/Includes/gui/k_mean_cluster_node.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'k_mean_cluster_node.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_k_mean_cluster_node_t {
    QByteArrayData data[7];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_k_mean_cluster_node_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_k_mean_cluster_node_t qt_meta_stringdata_k_mean_cluster_node = {
    {
QT_MOC_LITERAL(0, 0, 19), // "k_mean_cluster_node"
QT_MOC_LITERAL(1, 20, 9), // "preview_b"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 13), // "combo_changed"
QT_MOC_LITERAL(4, 45, 3), // "str"
QT_MOC_LITERAL(5, 49, 10), // "sb_changed"
QT_MOC_LITERAL(6, 60, 3) // "val"

    },
    "k_mean_cluster_node\0preview_b\0\0"
    "combo_changed\0str\0sb_changed\0val"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_k_mean_cluster_node[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x0a /* Public */,
       5,    1,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void k_mean_cluster_node::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<k_mean_cluster_node *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->preview_b(); break;
        case 1: _t->combo_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sb_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject k_mean_cluster_node::staticMetaObject = { {
    QMetaObject::SuperData::link<node::staticMetaObject>(),
    qt_meta_stringdata_k_mean_cluster_node.data,
    qt_meta_data_k_mean_cluster_node,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *k_mean_cluster_node::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *k_mean_cluster_node::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_k_mean_cluster_node.stringdata0))
        return static_cast<void*>(this);
    return node::qt_metacast(_clname);
}

int k_mean_cluster_node::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = node::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
