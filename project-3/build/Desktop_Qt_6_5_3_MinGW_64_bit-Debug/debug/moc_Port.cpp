/****************************************************************************
** Meta object code from reading C++ file 'Port.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Port/Port.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Port.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSPortENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSPortENDCLASS = QtMocHelpers::stringData(
    "Port",
    "packetSent",
    "",
    "QSharedPointer<Packet>",
    "packet",
    "packetReceived",
    "routerConnected",
    "portNumber",
    "connectedPortNumber",
    "routerDisconnected",
    "pcConnected",
    "QSharedPointer<HostAddress>",
    "pcAddress",
    "pcDisconnected"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPortENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[5];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[23];
    char stringdata4[7];
    char stringdata5[15];
    char stringdata6[16];
    char stringdata7[11];
    char stringdata8[20];
    char stringdata9[19];
    char stringdata10[12];
    char stringdata11[28];
    char stringdata12[10];
    char stringdata13[15];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPortENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPortENDCLASS_t qt_meta_stringdata_CLASSPortENDCLASS = {
    {
        QT_MOC_LITERAL(0, 4),  // "Port"
        QT_MOC_LITERAL(5, 10),  // "packetSent"
        QT_MOC_LITERAL(16, 0),  // ""
        QT_MOC_LITERAL(17, 22),  // "QSharedPointer<Packet>"
        QT_MOC_LITERAL(40, 6),  // "packet"
        QT_MOC_LITERAL(47, 14),  // "packetReceived"
        QT_MOC_LITERAL(62, 15),  // "routerConnected"
        QT_MOC_LITERAL(78, 10),  // "portNumber"
        QT_MOC_LITERAL(89, 19),  // "connectedPortNumber"
        QT_MOC_LITERAL(109, 18),  // "routerDisconnected"
        QT_MOC_LITERAL(128, 11),  // "pcConnected"
        QT_MOC_LITERAL(140, 27),  // "QSharedPointer<HostAddress>"
        QT_MOC_LITERAL(168, 9),  // "pcAddress"
        QT_MOC_LITERAL(178, 14)   // "pcDisconnected"
    },
    "Port",
    "packetSent",
    "",
    "QSharedPointer<Packet>",
    "packet",
    "packetReceived",
    "routerConnected",
    "portNumber",
    "connectedPortNumber",
    "routerDisconnected",
    "pcConnected",
    "QSharedPointer<HostAddress>",
    "pcAddress",
    "pcDisconnected"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPortENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   50,    2, 0x06,    1 /* Public */,
       5,    1,   53,    2, 0x06,    3 /* Public */,
       6,    2,   56,    2, 0x06,    5 /* Public */,
       9,    1,   61,    2, 0x06,    8 /* Public */,
      10,    2,   64,    2, 0x06,   10 /* Public */,
      13,    2,   69,    2, 0x06,   13 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    7,    8,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 11,    7,   12,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 11,    7,   12,

       0        // eod
};

Q_CONSTINIT const QMetaObject Port::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSPortENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPortENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPortENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Port, std::true_type>,
        // method 'packetSent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QSharedPointer<Packet>, std::false_type>,
        // method 'packetReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QSharedPointer<Packet>, std::false_type>,
        // method 'routerConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'routerDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'pcConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QSharedPointer<HostAddress>, std::false_type>,
        // method 'pcDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<QSharedPointer<HostAddress>, std::false_type>
    >,
    nullptr
} };

void Port::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Port *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->packetSent((*reinterpret_cast< std::add_pointer_t<QSharedPointer<Packet>>>(_a[1]))); break;
        case 1: _t->packetReceived((*reinterpret_cast< std::add_pointer_t<QSharedPointer<Packet>>>(_a[1]))); break;
        case 2: _t->routerConnected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->routerDisconnected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->pcConnected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QSharedPointer<HostAddress>>>(_a[2]))); break;
        case 5: _t->pcDisconnected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QSharedPointer<HostAddress>>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QSharedPointer<HostAddress> >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QSharedPointer<HostAddress> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Port::*)(QSharedPointer<Packet> );
            if (_t _q_method = &Port::packetSent; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Port::*)(QSharedPointer<Packet> );
            if (_t _q_method = &Port::packetReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Port::*)(int , int );
            if (_t _q_method = &Port::routerConnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Port::*)(int );
            if (_t _q_method = &Port::routerDisconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Port::*)(int , QSharedPointer<HostAddress> );
            if (_t _q_method = &Port::pcConnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Port::*)(int , QSharedPointer<HostAddress> );
            if (_t _q_method = &Port::pcDisconnected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject *Port::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Port::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPortENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QEnableSharedFromThis<Port>"))
        return static_cast< QEnableSharedFromThis<Port>*>(this);
    return QObject::qt_metacast(_clname);
}

int Port::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Port::packetSent(QSharedPointer<Packet> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Port::packetReceived(QSharedPointer<Packet> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Port::routerConnected(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Port::routerDisconnected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Port::pcConnected(int _t1, QSharedPointer<HostAddress> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Port::pcDisconnected(int _t1, QSharedPointer<HostAddress> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
