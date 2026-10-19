TARGET = cnca3d
APPNAME = cnca3
VERSION = 1.0.0
BUNDLE_VERSION = 6

# QMAKE_LFLAGS += -fuse-ld=lld

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


DISTFILES += \
    .clang-format \
    .clang-tidy \
    .gitignore \
    assets/config.json


include($$PWD/src/src.pri)

INCLUDEPATH += $$PWD/src/Globals \
               $$PWD/src/MACAddress \
               $$PWD/src/MACAddressGenerator \
               $$PWD/Packet \
               $$PWD/DataGenerator \
               $$PWD/ConfigurationManager \
               $$PWD/EventsCoordinator \
               $$PWD/IP \
               $$PWD/AbstractIP \
               $$PWD/IPV4 \
               $$PWD/IPV6 \
               $$PWD/IPHeader \
               $$PWD/IPV4Header \
               $$PWD/IPV6Header \
               $$PWD/Node \
               $$PWD/Router \
               $$PWD/PC \
               $$PWD/Port \
               $$PWD/PortBindingManager \
               $$PWD/HostAddress \
               $$PWD/Node \
               $$PWD/Router \
               $$PWD/PC \
               $$PWD/TopologyBuilder \
               $$PWD/TopologyController \
               $$PWD/AutonomousSystem \
               $$PWD/Ospf
