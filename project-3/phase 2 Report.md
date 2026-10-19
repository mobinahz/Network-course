# Code Documentation
## 1. Port Class
**Purpose:**
The `Port` class simulates the operational functionalities of a network port. It is designed to manage packet sending and receiving, enabling real-time communication in the simulated environment.

**Key Features:**
- **Packet Transmission:** Handles sending packets with signal emission to notify the system.
- **Packet Reception:** Receives packets and emits a signal to notify the system of incoming data.
- **Connectivity Management:** Allows dynamic connection and disconnection to other ports, mimicking real-world network flexibility.

**Implementation Details:**
- **Signals:** `packetSent` and `packetReceived` to notify other components of packet transmission and reception.
- **Public Slots:** `sendPacket` and `receivePacket` that manage the data flow through the port.

## 2. PortBindingManager Class
**Purpose:**
`PortBindingManager` oversees the logical connections between network ports within the simulation, crucial for setting up and modifying network topologies dynamically.

**Key Features:**
- **Dynamic Binding:** Facilitates the binding of two ports, enabling bidirectional communication.
- **Dynamic Unbinding:** Allows ports to be unbound, supporting dynamic network topology changes.

**Implementation Details:**
- **Connection Management:** Manages a mapping of connections that ensures proper routing of packets between bound ports.
- **Signal Handling:** Uses Qt signals to synchronize port binding states across the simulation.

## 3. Node Class
**Purpose:**
The `Node` class acts as a superclass for all network devices in the simulation, providing essential networking capabilities and serving as the foundation for specialized device classes like PCs and routers.

**Key Features:**
- **Network Identification:** Stores and manages basic network identifiers like IP and MAC addresses.
- **Thread Functionality:** Incorporates threading to simulate concurrent network processes.

**Implementation Details:**
- **Base Attributes:** Includes identifiers and state necessary for basic networking tasks.
- **Threaded Behavior:** Implements a virtual `run` method for derived classes to define specific operational behaviors.

## 4. PC Class
**Purpose:**
Derived from `Node`, the `PC` class represents a personal computer within the simulated network, equipped to handle specific data transmission tasks triggered by network events.

**Key Features:**
- **Event-Driven Communication:** Reacts to external ticks to simulate data transmission activities.
- **Network Mapping:** Maintains a map of network peers' IP and MAC addresses for packet routing.

**Implementation Details:**
- **Single Port:** Manages a single network port for all outgoing and incoming communications.
- **Handling Network Events:** Slots to respond to data generation signals, simulating real-world PC behavior in response to network traffic.

## 5. Router Class
**Purpose:**
The `Router` class extends `Node` to simulate a network router, capable of directing traffic between multiple network segments based on a configurable routing table.

**Key Features:**
- **Multiple Ports:** Contains four ports to simulate interfaces connected to different network segments.
- **Routing Table:** Manages a table mapping destination IPs to corresponding ports for packet forwarding.

**Implementation Details:**
- **Port Management:** Handles four distinct network ports, each potentially connected to different parts of the simulated network.
- **Packet Routing:** Uses the routing table to determine the appropriate port for outgoing packets, simulating standard routing mechanisms in networks.
