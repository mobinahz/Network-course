/****************************************************************************
** Meta object code from reading C++ file 'Router.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/Router/Router.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Router.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSRouterENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSRouterENDCLASS = QtMocHelpers::stringData(
    "Router",
    "dhcpOfferSignal",
    "",
    "DHCPRequest",
    "request",
    "QSharedPointer<HostAddress>",
    "offeredIP",
    "dhcpAckSignal",
    "assignedIP",
    "onClockTick",
    "onDhcpDiscoverFromPC",
    "onDhcpRequestFromPC",
    "onDhcpAckReceived"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSRouterENDCLASS_t {
    uint offsetsAndSizes[26];
    char stringdata0[7];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[8];
    char stringdata5[28];
    char stringdata6[10];
    char stringdata7[14];
    char stringdata8[11];
    char stringdata9[12];
    char stringdata10[21];
    char stringdata11[20];
    char stringdata12[18];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSRouterENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSRouterENDCLASS_t qt_meta_stringdata_CLASSRouterENDCLASS = {
    {
        QT_MOC_LITERAL(0, 6),  // "Router"
        QT_MOC_LITERAL(7, 15),  // "dhcpOfferSignal"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 11),  // "DHCPRequest"
        QT_MOC_LITERAL(36, 7),  // "request"
        QT_MOC_LITERAL(44, 27),  // "QSharedPointer<HostAddress>"
        QT_MOC_LITERAL(72, 9),  // "offeredIP"
        QT_MOC_LITERAL(82, 13),  // "dhcpAckSignal"
        QT_MOC_LITERAL(96, 10),  // "assignedIP"
        QT_MOC_LITERAL(107, 11),  // "onClockTick"
        QT_MOC_LITERAL(119, 20),  // "onDhcpDiscoverFromPC"
        QT_MOC_LITERAL(140, 19),  // "onDhcpRequestFromPC"
        QT_MOC_LITERAL(160, 17)   // "onDhcpAckReceived"
    },
    "Router",
    "dhcpOfferSignal",
    "",
    "DHCPRequest",
    "request",
    "QSharedPointer<HostAddress>",
    "offeredIP",
    "dhcpAckSignal",
    "assignedIP",
    "onClockTick",
    "onDhcpDiscoverFromPC",
    "onDhcpRequestFromPC",
    "onDhcpAckReceived"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSRouterENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   50,    2, 0x06,    1 /* Public */,
       7,    2,   55,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,   60,    2, 0x0a,    7 /* Public */,
      10,    1,   61,    2, 0x0a,    8 /* Public */,
      11,    1,   64,    2, 0x0a,   10 /* Public */,
      12,    2,   67,    2, 0x0a,   12 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    8,

       0        // eod
};

Q_CONSTINIT const QMetaObject Router::staticMetaObject = { {
    QMetaObject::SuperData::link<Node::staticMetaObject>(),
    qt_meta_stringdata_CLASSRouterENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSRouterENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSRouterENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Router, std::true_type>,
        // method 'dhcpOfferSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DHCPRequest &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QSharedPointer<HostAddress> &, std::false_type>,
        // method 'dhcpAckSignal'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DHCPRequest &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QSharedPointer<HostAddress> &, std::false_type>,
        // method 'onClockTick'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDhcpDiscoverFromPC'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DHCPRequest &, std::false_type>,
        // method 'onDhcpRequestFromPC'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DHCPRequest &, std::false_type>,
        // method 'onDhcpAckReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DHCPRequest &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QSharedPointer<HostAddress> &, std::false_type>
    >,
    nullptr
} };

void Router::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Router *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->dhcpOfferSignal((*reinterpret_cast< std::add_pointer_t<DHCPRequest>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QSharedPointer<HostAddress>>>(_a[2]))); break;
        case 1: _t->dhcpAckSignal((*reinterpret_cast< std::add_pointer_t<DHCPRequest>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QSharedPointer<HostAddress>>>(_a[2]))); break;
        case 2: _t->onClockTick(); break;
        case 3: _t->onDhcpDiscoverFromPC((*reinterpret_cast< std::add_pointer_t<DHCPRequest>>(_a[1]))); break;
        case 4: _t->onDhcpRequestFromPC((*reinterpret_cast< std::add_pointer_t<DHCPRequest>>(_a[1]))); break;
        case 5: _t->onDhcpAckReceived((*reinterpret_cast< std::add_pointer_t<DHCPRequest>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QSharedPointer<HostAddress>>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QSharedPointer<HostAddress> >(); break;
            }
            break;
        case 1:
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
            using _t = void (Router::*)(const DHCPRequest & , const QSharedPointer<HostAddress> & );
            if (_t _q_method = &Router::dhcpOfferSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Router::*)(const DHCPRequest & , const QSharedPointer<HostAddress> & );
            if (_t _q_method = &Router::dhcpAckSignal; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *Router::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Router::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSRouterENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return Node::qt_metacast(_clname);
}

int Router::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Node::qt_metacall(_c, _id, _a);
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
void Router::dhcpOfferSignal(const DHCPRequest & _t1, const QSharedPointer<HostAddress> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Router::dhcpAckSignal(const DHCPRequest & _t1, const QSharedPointer<HostAddress> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
