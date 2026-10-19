/****************************************************************************
** Meta object code from reading C++ file 'PC.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/PC/PC.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QSharedPointer>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PC.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSPCENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSPCENDCLASS = QtMocHelpers::stringData(
    "PC",
    "sendPacket",
    "",
    "QSharedPointer<Packet>",
    "packet",
    "generateAndSendPackets",
    "numberOfPackets",
    "onDhcpOfferReceived",
    "DHCPRequest",
    "request",
    "QSharedPointer<HostAddress>",
    "offeredIP",
    "onDhcpAckReceived",
    "assignedIP"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPCENDCLASS_t {
    uint offsetsAndSizes[28];
    char stringdata0[3];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[23];
    char stringdata4[7];
    char stringdata5[23];
    char stringdata6[16];
    char stringdata7[20];
    char stringdata8[12];
    char stringdata9[8];
    char stringdata10[28];
    char stringdata11[10];
    char stringdata12[18];
    char stringdata13[11];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPCENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPCENDCLASS_t qt_meta_stringdata_CLASSPCENDCLASS = {
    {
        QT_MOC_LITERAL(0, 2),  // "PC"
        QT_MOC_LITERAL(3, 10),  // "sendPacket"
        QT_MOC_LITERAL(14, 0),  // ""
        QT_MOC_LITERAL(15, 22),  // "QSharedPointer<Packet>"
        QT_MOC_LITERAL(38, 6),  // "packet"
        QT_MOC_LITERAL(45, 22),  // "generateAndSendPackets"
        QT_MOC_LITERAL(68, 15),  // "numberOfPackets"
        QT_MOC_LITERAL(84, 19),  // "onDhcpOfferReceived"
        QT_MOC_LITERAL(104, 11),  // "DHCPRequest"
        QT_MOC_LITERAL(116, 7),  // "request"
        QT_MOC_LITERAL(124, 27),  // "QSharedPointer<HostAddress>"
        QT_MOC_LITERAL(152, 9),  // "offeredIP"
        QT_MOC_LITERAL(162, 17),  // "onDhcpAckReceived"
        QT_MOC_LITERAL(180, 10)   // "assignedIP"
    },
    "PC",
    "sendPacket",
    "",
    "QSharedPointer<Packet>",
    "packet",
    "generateAndSendPackets",
    "numberOfPackets",
    "onDhcpOfferReceived",
    "DHCPRequest",
    "request",
    "QSharedPointer<HostAddress>",
    "offeredIP",
    "onDhcpAckReceived",
    "assignedIP"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPCENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    1,   41,    2, 0x0a,    3 /* Public */,
       7,    2,   44,    2, 0x0a,    5 /* Public */,
      12,    2,   49,    2, 0x0a,    8 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 10,    9,   11,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 10,    9,   13,

       0        // eod
};

Q_CONSTINIT const QMetaObject PC::staticMetaObject = { {
    QMetaObject::SuperData::link<Node::staticMetaObject>(),
    qt_meta_stringdata_CLASSPCENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPCENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPCENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PC, std::true_type>,
        // method 'sendPacket'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QSharedPointer<Packet> &, std::false_type>,
        // method 'generateAndSendPackets'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onDhcpOfferReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DHCPRequest &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QSharedPointer<HostAddress> &, std::false_type>,
        // method 'onDhcpAckReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const DHCPRequest &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QSharedPointer<HostAddress> &, std::false_type>
    >,
    nullptr
} };

void PC::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PC *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendPacket((*reinterpret_cast< std::add_pointer_t<QSharedPointer<Packet>>>(_a[1]))); break;
        case 1: _t->generateAndSendPackets((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->onDhcpOfferReceived((*reinterpret_cast< std::add_pointer_t<DHCPRequest>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QSharedPointer<HostAddress>>>(_a[2]))); break;
        case 3: _t->onDhcpAckReceived((*reinterpret_cast< std::add_pointer_t<DHCPRequest>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QSharedPointer<HostAddress>>>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 1:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QSharedPointer<HostAddress> >(); break;
            }
            break;
        case 3:
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
            using _t = void (PC::*)(const QSharedPointer<Packet> & );
            if (_t _q_method = &PC::sendPacket; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *PC::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PC::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPCENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return Node::qt_metacast(_clname);
}

int PC::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Node::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void PC::sendPacket(const QSharedPointer<Packet> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
