QT     += core gui network
CONFIG += lrelease
CONFIG += no_keywords
CONFIG += c++20
INCLUDEPATH += $$PWD/Globals \
               $$PWD/MACAddress \
               $$PWD/MACAddressGenerator \
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
               $$PWD/AutonomousSystem

SOURCES += \
    $$PWD/AbstractIP/AbstractIP.cpp \
    $$PWD/AutonomousSystem/AutonomousSystem.cpp \
    $$PWD/ConfigurationManager/ConfigurationManager.cpp \
    $$PWD/DHCP/DHCP.cpp \
    $$PWD/DHCPServer/DHCPServer.cpp \
    $$PWD/DataGenerator/DataGenerator.cpp \
    $$PWD/DataLinkHeader/DataLinkHeader.cpp \
    $$PWD/EventsCoordinator/EventsCoordinator.cpp \
    $$PWD/HostAddress/HostAddress.cpp \
    $$PWD/IP/IP.cpp \
    $$PWD/IPHeader/IPHeader.cpp \
    $$PWD/IPV4/IPV4.cpp \
    $$PWD/IPV4Header/IPV4Header.cpp \
    $$PWD/IPV6/IPV6.cpp \
    $$PWD/IPV6Header/IPV6Header.cpp \
    $$PWD/Logger/Logger.cpp \
    $$PWD/MACAddress/MACAddress.cpp \
    $$PWD/MACAddressGenerator/MACAddressGenerator.cpp \
    $$PWD/Network/Network.cpp \
    $$PWD/Node/Node.cpp \
    $$PWD/OSPF/ospf.cpp \
    $$PWD/PC/PC.cpp \
    $$PWD/Packet/Packet.cpp \
    $$PWD/Port/Port.cpp \
    $$PWD/PortBindingManager/PortBindingManager.cpp \
    $$PWD/RIP/RIP.cpp \
    $$PWD/Router/Router.cpp \
    $$PWD/Simulator/Simulator.cpp \
    $$PWD/TCPHeader/TCPHeader.cpp \
    $$PWD/TopologyBuilder/TopologyBuilder.cpp \
    $$PWD/TopologyController/TopologyController.cpp \
    $$PWD/main.cpp

HEADERS += \
    $$PWD/AbstractIP/AbstractIP.h \
    $$PWD/AutonomousSystem/AutonomousSystem.h \
    $$PWD/ConfigurationManager/ConfigurationManager.h \
    $$PWD/DHCP/DHCP.h \
    $$PWD/DHCPServer/DHCPServer.h \
    $$PWD/DataGenerator/DataGenerator.h \
    $$PWD/DataLinkHeader/DataLinkHeader.h \
    $$PWD/EventsCoordinator/EventsCoordinator.h \
    $$PWD/Globals/Globals.h \
    $$PWD/HostAddress/HostAddress.h \
    $$PWD/IP/IP.h \
    $$PWD/IPHeader/IPHeader.h \
    $$PWD/IPV4/IPV4.h \
    $$PWD/IPV4Header/IPV4Header.h \
    $$PWD/IPV6/IPV6.h \
    $$PWD/IPV6Header/IPV6Header.h \
    $$PWD/Logger/Logger.h \
    $$PWD/MACAddress/MACAddress.h \
    $$PWD/MACAddressGenerator/MACAddressGenerator.h \
    $$PWD/Network/Network.h \
    $$PWD/Node/Node.h \
    $$PWD/OSPF/ospf.h \
    $$PWD/PC/PC.h \
    $$PWD/Packet/Packet.h \
    $$PWD/Port/Port.h \
    $$PWD/PortBindingManager/PortBindingManager.h \
    $$PWD/RIP/RIP.h \
    $$PWD/Router/Router.h \
    $$PWD/Simulator/Simulator.h \
    $$PWD/TCPHeader/TCPHeader.h \
    $$PWD/TopologyBuilder/TopologyBuilder.h \
    $$PWD/TopologyController/TopologyController.h \
    $$PWD/json.hpp
