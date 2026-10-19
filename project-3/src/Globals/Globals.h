#ifndef GLOBALS_H
#define GLOBALS_H

namespace UT
{

enum class IPVersion
{
    IPv4,
    IPv6
};

enum class PacketType
{
    Data,
    Control
};

enum class PacketControlType
{
    Request,
    Response,
    Acknowledge,
    Error,
    DHCPDiscovery,
    DHCPOffer,
    DHCPRequest,
    DHCPAcknowledge,
    DHCPNak,
    RIP,
    OSPF,
};

enum class DistributionType
{
    Poisson,
    Pareto
};

enum class TopologyType
{
    Mesh,
    RingStar,
    Torus
};

enum class GatewayType
{
    as_gateway,
    user_gateway
};

enum class Protocol
{
    RIP,
    OSPF,
    BGP
};

enum class PortConnectionType
{
    Router,
    User
};
}

#endif    // GLOBALS_H
