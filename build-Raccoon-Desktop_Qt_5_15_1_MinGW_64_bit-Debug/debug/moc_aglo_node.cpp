/****************************************************************************
** Meta object code from reading C++ file 'aglo_node.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Raccoon/Includes/gui/aglo_node.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'aglo_node.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_aglo_node_t {
    QByteArrayData data[12];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_aglo_node_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_aglo_node_t qt_meta_stringdata_aglo_node = {
    {
QT_MOC_LITERAL(0, 0, 9), // "aglo_node"
QT_MOC_LITERAL(1, 10, 9), // "preview_b"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 13), // "combo_changed"
QT_MOC_LITERAL(4, 35, 3), // "str"
QT_MOC_LITERAL(5, 39, 19), // "combo_clust_changed"
QT_MOC_LITERAL(6, 59, 12), // "dist_changed"
QT_MOC_LITERAL(7, 72, 1), // "v"
QT_MOC_LITERAL(8, 74, 11), // "num_changed"
QT_MOC_LITERAL(9, 86, 13), // "scene_changed"
QT_MOC_LITERAL(10, 100, 13), // "QList<QRectF>"
QT_MOC_LITERAL(11, 114, 6) // "region"

    },
    "aglo_node\0preview_b\0\0combo_changed\0"
    "str\0combo_clust_changed\0dist_changed\0"
    "v\0num_changed\0scene_changed\0QList<QRectF>\0"
    "region"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_aglo_node[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    1,   45,    2, 0x0a /* Public */,
       5,    1,   48,    2, 0x0a /* Public */,
       6,    1,   51,    2, 0x0a /* Public */,
       8,    1,   54,    2, 0x0a /* Public */,
       9,    1,   57,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void aglo_node::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<aglo_node *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->preview_b(); break;
        case 1: _t->combo_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->combo_clust_changed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->dist_changed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->num_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->scene_changed((*reinterpret_cast< const QList<QRectF>(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QRectF> >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject aglo_node::staticMetaObject = { {
    QMetaObject::SuperData::link<node::staticMetaObject>(),
    qt_meta_stringdata_aglo_node.data,
    qt_meta_data_aglo_node,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *aglo_node::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *aglo_node::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_aglo_node.stringdata0))
        return static_cast<void*>(this);
    return node::qt_metacast(_clname);
}

int aglo_node::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = node::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE