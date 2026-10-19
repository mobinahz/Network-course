Computer Networks - CA3 - Fall1403

Arshiya Ataei - Mobina Haghizadeh

810100252 - 810100127
--------------------------------------------------------------
# Phase 1
## Overview
This phase focuses on implementing foundational components for a network simulation system. The primary goal is to establish key data structures and utilities required for simulating packet transmission and protocol behavior.

## Features
- MacAddress Class
- MacAddressGenerator Class
- DataGenarator Class
- EventsCoordinator Class
- Packet Class
- DataLinkHeader Class
- TCPHeader Class
  
-----------------------------------------------------------

# Code Documentation
## 1. MacAddress Class
This MACAddress class provides a neat way to handle MAC addresses in an application. When we create a new instance, we can pass in a MAC string (like aa:bb:cc:dd:ee:ff) or leave it empty. If it’s empty, it picks a random MAC address for us. Internally, it stores the MAC as six bytes and can quickly turn that into a nice, readable QString. It also lets us compare two MAC addresses with == or !=. Essentially, it ensures working with MAC addresses in code is convenient and consistent.

## 2. MacAddressGenerator Class
This MACAddressGenerator takes an initial MAC address and, from that point, hands out a sequence of unique addresses one after another. Each time we ask for the next address, it returns the current one and then updates its internal counter to move on to the next address in the sequence. Under the hood, it’s basically treating the MAC address like a number, adding one each time, and handling the rollover when it reaches the end of its range. In other words, it makes generating a continuous set of MAC addresses easy.

## 3. DataGenarator Class
DataGenerator is a class that produces a small batch of packets each time it’s asked, choosing their sizes based on a Pareto distribution. It pulls randomness from QRandomGenerator to give each packet a unique source, a unique destination, and a size that’s more than just a simple guess. It doesn’t just hand out uniform values; it dips into probability to create more realistic packet sizes.

## 4. EventsCoordinator Class
EventsCoordinator is like the central conductor that runs on its own thread, periodically waking up to produce fresh data through a provided DataGenerator. It manages a timer, kicks off the simulation, stops it when asked, and sends out the resulting packets to whoever’s interested. By handling all this in its own thread, it keeps timing and data generation neatly organized and responsive.

## 5. Packet Class
Purpose:
The Packet class is the fundamental data structure representing a network packet. It encapsulates the core properties and metadata of a packet transmitted within the network.

Key Features
Source and Destination: Stores the origin and target of the packet, represented as strings.
Size: Tracks the size of the packet in bytes.
Extensibility: Designed to include additional properties (e.g., payload, headers) in future enhancements.
Implementation Details
Attributes: source, destination, and size.
Constructors: Allows initialization with source, destination, and size values.
Getters and Setters: Provides methods to access and modify the attributes.
The Packet class simplifies packet management by providing a lightweight structure to store and manipulate network packet data.

## 6. DataLinkHeader Class
Purpose:
The DataLinkHeader class represents the header at the data link layer of the network protocol stack. It includes essential fields for managing packet transmission at this layer.

Key Features
Source and Destination MAC Addresses: Represents the Media Access Control (MAC) addresses for packet transmission.
Frame Type: Indicates the type of frame being transmitted (e.g., IPv4, ARP).
Header Size: A fixed size of 14 bytes (standard for Ethernet).
Implementation Details
Attributes:
sourceMac: MAC address of the sender.
destinationMac: MAC address of the recipient.
frameType: An integer representing the frame type.
Constructors: Initializes the header fields with appropriate values.
Getters and Setters: Provides access to modify the header fields.
Extensibility: Can be extended for additional link-layer protocols or features.
This class provides a structured way to manage data link layer information, ensuring compliance with Ethernet standards.

## 7. TCPHeader Class
Purpose:
The TCPHeader class represents the header of a Transmission Control Protocol (TCP) segment. It manages key fields necessary for TCP communication, including port numbers, sequence numbers, and control flags.

Key Features
Source and Destination Ports: Identifies the sending and receiving applications.
Sequence and Acknowledgment Numbers: Tracks the order and receipt of packets.
Flags: Manages TCP control flags (e.g., SYN, ACK, FIN).
Window Size: Controls flow of data by indicating the size of the receive window.
Checksum: Ensures data integrity during transmission.
Urgent Pointer: Points to urgent data when the URG flag is set.
Implementation Details
Attributes:
sourcePort and destinationPort: 16-bit fields.
sequenceNumber and ackNumber: 32-bit fields.
dataOffset, flags, windowSize, checksum, and urgentPointer: Representing other TCP header components.
Constructors: Initializes all fields of the TCP header.
Getters and Setters: Provides methods to access and modify the header fields.
Serialization: Converts the header into a byte array for packet transmission.
The TCPHeader class follows the standard TCP header structure, supporting extensibility for TCP options if required in the future.

--------------------------------------------------------------
## Relation Between EventsCoordinator and DataGenerator
The `EventsCoordinator` and `DataGenerator` classes form a dynamic partnership in driving the simulation forward. The `EventsCoordinator` acts as the central conductor, managing the flow of time in the simulation through its timer-based cycles. It is responsible for triggering updates across the system, ensuring that each simulation cycle progresses seamlessly.

On the other hand, the `DataGenerator` is the creative engine of this setup. It produces packets that represent network traffic, generating the data that flows through the simulation. Each cycle begins with the `EventsCoordinator` signaling the `DataGenerator` to create a fresh batch of packets. These packets, filled with information like source, destination, and size, are handed back to the `EventsCoordinator`.

With the packets in hand, the `EventsCoordinator` broadcasts them to the rest of the network. It emits signals to inform other components—such as network nodes or devices—that it’s time to process and transmit the newly generated packets. This cycle repeats at regular intervals, creating a rhythm for the simulation that mimics real-world network activity.

This relationship is designed to be flexible. The `DataGenerator` focuses purely on creating data, while the `EventsCoordinator` takes on the responsibility of orchestrating the entire simulation. By separating these roles, the system is both modular and scalable, ready to accommodate more complex behavior as the simulation evolves. This interplay ensures that the simulation feels both dynamic and realistic, with each component playing its part in the larger system.

--------------------------------------------------------------


# Phase2
## Overview
In **Phase Two**, the focus shifts from foundational components to building the network's structural and operational backbone. This phase emphasizes the creation of dynamic entities like **IP**, **Ports**, and **Nodes**, which form the essential elements of communication in the system. **TopologyBuilder** and **TopologyController** provide the ability to create, manage, and modify network topologies in real time, enabling the simulation of complex network layouts such as Mesh, Torus, and RingStar.

The introduction of the **AutonomousSystem** class allows for higher-level abstraction, grouping devices into independent systems with their own topologies and interconnections. By integrating traffic through **DataGenerator** and managing realistic packet exchanges between PCs and Routers, this phase lays the groundwork for simulating scalable, multi-system networks with dynamic topological and traffic patterns.

## Features
- IP
- IPHeader
- Port
- PortBindingManager
- Node
- PC
- Router (without routing algorithms)
- TopologyBuilder
- TopologyController
- AutonomousSystem
  
-----------------------------------------------------------


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

## 6. IPHeader Class

The `IPHeader` class models the header of an IP packet in the simulation, encapsulating details necessary for packet handling and transmission. It is an abstract base class that provides essential functionality such as version retrieval, header validation, serialization (`toByteArray`) and deserialization (`fromByteArray`), and conversion to a human-readable string format (`toString`).

Key methods include `isValid` for ensuring header integrity and `clear` for resetting its state. It emits an `errorOccurred` signal to handle issues during operations, making it a foundational component for implementing IP-specific logic like IPv4 or IPv6 in the simulation.

## 7. IP Class

### Overview of IP Handling System

In our system, IP address management is designed to handle both IPv4 and IPv6 addresses efficiently while providing flexibility and extensibility. The architecture leverages an object-oriented approach with a hierarchy of classes, where the `AbstractIP` class serves as the foundation. This design ensures modularity, reusability, and clean abstraction for IP-related operations.

---

#### **AbstractIP**
The `AbstractIP` class is the base class for all IP address representations. It defines a common interface for IP-specific functionality such as converting an address to a string, parsing an address from a string, and determining address characteristics like loopback or multicast status. It ensures consistent behavior across different IP types and acts as the parent class for specialized implementations.

Key functionalities include:
- Defining virtual methods for essential operations (`toString`, `fromString`, etc.).
- Serving as a polymorphic base to allow seamless handling of both IPv4 and IPv6 through shared pointers in higher-level classes like `HostAddress`.

---

#### **IPV4**
The `IPV4` class inherits from `AbstractIP` and implements functionality specific to IPv4 addresses. It uses a 32-bit representation for storing IP addresses and provides methods to parse and validate IPv4 addresses from strings, convert them back to string format, and check for special address types (e.g., loopback, multicast).

The `IPV4` class handles:
- Conversion between numeric and string representations of IPv4 addresses.
- Validation of IPv4 formats during parsing.
- Identifying special IPv4 address types (e.g., loopback addresses in the `127.0.0.0/8` range and multicast addresses in the `224.0.0.0/4` range).

---

#### **IPV6**
The `IPV6` class also inherits from `AbstractIP` but is tailored for IPv6 addresses, which are 128-bit in size. It uses a 16-byte array (`std::array<uint8_t, 16>`) to store the address and implements methods for parsing, validation, and string conversion specific to IPv6.

The `IPV6` class focuses on:
- Handling IPv6 address strings with colons (`:`) and compressed formats (e.g., `::1` for loopback).
- Checking special IPv6 types such as multicast (`FF00::/8`) and loopback (`::1`).
- Supporting conversions and operations tailored for the IPv6 protocol while ensuring compatibility with the `AbstractIP` interface.

---

#### **HostAddress**
The `HostAddress` class acts as a high-level abstraction that manages both IPv4 and IPv6 addresses dynamically. It uses a `QSharedPointer<AbstractIP>` to store either an `IPV4` or `IPV6` instance, allowing seamless interaction with either protocol without requiring separate handling logic.

The `HostAddress` class provides:
- Dynamic IP type management, determining whether an address is IPv4 or IPv6 based on its format or explicit initialization.
- Utility methods for address validation, subnet matching, and protocol identification.
- Integration with higher-level components such as routers and nodes, abstracting away the complexities of handling different IP versions.

The `HostAddress` class also includes constructors for initializing from special addresses, such as `localhost` or `broadcast`, and supports operations like comparing addresses and checking network membership.

---

### Key Strengths of the System
- **Modularity**: Each IP version is encapsulated in its own class, allowing clear separation of concerns.
- **Extensibility**: New IP protocols can be added by extending `AbstractIP` without modifying existing classes.
- **Unified Interface**: The `HostAddress` class provides a consistent API for working with IP addresses, abstracting the differences between IPv4 and IPv6.
- **Dynamic Handling**: Through polymorphism and shared pointers, the system dynamically handles both IPv4 and IPv6 addresses in a single, unified structure.

This architecture balances flexibility and clarity, making it easy to integrate IP operations into broader network simulations or applications.


## 8. TopologyBuilder class

The `TopologyBuilder` class is responsible for creating and configuring network topologies for Autonomous Systems (AS) in the simulation. It dynamically builds and connects routers based on topology specifications from the configuration, such as **Mesh**, **Torus**, and **RingStar**.

Using the `buildTopology` method, it retrieves AS-specific parameters like the number of nodes, broken routers, and topology type from the `ConfigurationManager`. It then instantiates routers, skips broken ones, and applies the selected topology by invoking specific methods (`setupMeshTopology`, `setupTorusTopology`, or `setupRingStarTopology`). Each topology setup function connects routers in the desired pattern, ensuring realistic simulation of network layouts.

The `TopologyBuilder` is highly dynamic and relies on modular methods to adapt to different configurations, making it a crucial component for scalable and flexible network simulations.

## 9. AutonomousSystem class

The `AutonomousSystem` class represents a self-contained network domain, responsible for managing its internal topology, devices, and interconnections with other systems. It uses configuration data to dynamically initialize its routers and PCs, construct topologies via the `TopologyController`, and establish internal and external connections. 

A key method, `initializeTopology`, retrieves AS-specific parameters (e.g., node count, topology type) and sets up the network using routers managed by the controller. It also integrates traffic generation by associating `DataGenerator` instances with PCs, enabling realistic simulation of packet flows within the AS. The class ensures modularity, scalability, and adaptability to dynamic network scenarios.

## 10. TopologyController class

The `TopologyController` class serves as a high-level manager for configuring and dynamically modifying network topologies within an Autonomous System (AS). It integrates with the `TopologyBuilder` and `ConfigurationManager` to initialize, switch, or reset topologies based on configuration data or runtime requirements.

A key method, `initTopologyForAS`, initializes an AS's topology by leveraging `TopologyBuilder` to create routers and establish connections based on the AS's configuration. It then applies a specific topology (e.g., Mesh, Torus) using `switchTopology`, allowing for runtime flexibility. The class also provides mechanisms to reset topologies or signal when changes are applied, ensuring adaptability and coordination in dynamic network simulations.

## Relation Between TopologyBuilder, TopologyController and AutonomousSystem

The `TopologyBuilder`, `TopologyController`, and `AutonomousSystem` work together to create, manage, and dynamically configure the network topology within an Autonomous System. The `TopologyBuilder` constructs the initial topology based on configuration details, while the `TopologyController` orchestrates changes to the topology, such as switching layouts or resetting connections during runtime. The `AutonomousSystem` uses these tools to manage its internal devices (routers and PCs) and ensure seamless integration with other systems, enabling flexible and scalable network simulations.


-------------------------------------------------------------------------------------------------------------

# Phase 3

## Overview
In **phase 3** of your network simulator project, the focus is on fully integrating and testing the dynamic routing functionalities along with DHCP server capabilities within the simulated network environment. This phase builds on the foundational components developed in the earlier stages, bringing together the practical application of two critical dynamic routing protocols: RIP (Routing Information Protocol) and OSPF (Open Shortest Path First). Both protocols are key to managing how data packets are efficiently routed across different nodes and autonomous systems in the network depending on the network's topology and traffic conditions.

Alongside the routing protocols, this phase also integrates a DHCP Server into the network. The DHCP Server automates the assignment of IP addresses, subnet masks, and other network configuration parameters to nodes within the network. This allows for dynamic network changes and ease of management, mimicking real-world network conditions where devices frequently join and leave networks.

Finally, the entire Network is brought together to function as a cohesive unit, with the integrated components—RIP, OSPF, and the DHCP Server—working in harmony to simulate a realistic network environment. This phase is critical for testing the robustness and efficiency of the network under various conditions and ensuring that all parts of the simulator work together seamlessly to handle network tasks and traffic effectively.

## Features
- RIP
- OSPF
- DHCPServer
- Network

-----------------------------------------------------------


# Code Documentation
## 1. RIP class

In my project, I've implemented a RIP class to handle the dynamic routing within my network simulator. This class manages a routing table that records paths to various network destinations, including details like destination addresses, subnet masks, next-hop gateways, and the cost associated with each route.

My RIP class is integral to simulating how routers communicate within the network to update each other about the best routes for transmitting data. It periodically sends out the router's entire routing table to its neighbors and processes any incoming routing updates to adjust its own table accordingly. This process helps ensure that the routing information is both accurate and efficient, adapting dynamically to changes in the network's topology.

To support these operations, I've equipped the RIP class with functions that allow it to initialize the router's configurations, send and receive routing updates, and efficiently manage the routing table to avoid potential routing loops and ensure stability in the network communication. This setup allows me to simulate a realistic network environment where routing decisions are constantly optimized based on the latest available data.


## 2. OSPF class

In my project, I've integrated an OSPF class to simulate the Open Shortest Path First (OSPF) routing protocol within my network simulator. This class is pivotal in managing the OSPF operations which include maintaining a link-state database (LSDB) and executing the Dijkstra algorithm to compute the shortest paths across the network.

My OSPF class is set up to dynamically discover and manage network topology changes. It starts by identifying direct neighbors and their connection costs, which forms the base entries of the LSDB. From there, the class actively sends and receives Link State Advertisements (LSAs) to and from neighboring routers. This flooding mechanism ensures that all routers in the network have a synchronized view of the network topology.

Once the LSDB is updated with the latest data, my OSPF class runs the Dijkstra algorithm to determine the shortest path to all other nodes. This information is then used to update the router's routing table, optimizing the paths for data packets based on the most recent network state.

Additionally, I've implemented functionalities to print or log the LSDB, which is crucial for debugging and ensuring the accuracy of the OSPF implementation. This setup allows my network simulator to emulate a realistic OSPF-based routing environment, adjusting dynamically to the network conditions and maintaining efficient routing throughout.

## 3. DHCPServer class

In my project, I've developed a DHCP class to manage the dynamic host configuration protocol within my network simulator. This class is crucial for automating the network management process by dynamically assigning IP addresses and other network configurations to nodes within the network.

The DHCP class operates independently but can be integrated into any router within the network. It supports both IPv4 and IPv6 address assignments, reflecting the dual-stack capabilities needed in modern network environments. The class keeps track of all IP addresses it has assigned through a mapping of MAC addresses to HostAddress instances, ensuring that each device receives a consistent IP address if it reconnects to the network.

My DHCP implementation is designed to handle typical DHCP interactions—specifically, the discovery and request phases. When a node broadcasts a DHCP discover message, my DHCP server responds with an offer that includes an available IP address. If the node accepts the offer by sending a DHCP request, my server finalizes the assignment with a DHCP acknowledgment, effectively leasing the IP address to the node.

Additionally, I've added functionality to save and load DHCP records to and from a file. This feature is particularly useful for maintaining state across simulator restarts or for debugging purposes. The DHCP server can enable or disable its functionality dynamically, allowing for flexible testing scenarios within my simulator.

The inclusion of methods to log assignments and manage the lease lifecycle adds robustness to the DHCP simulation, ensuring that network administrators can understand and verify the behavior of the network under various conditions.

## 4. Network class


In this project, the Network class acts as the central hub for managing and interconnecting multiple autonomous systems (ASes). This class encapsulates all the high-level functionalities required for setting up, connecting, and managing the ASes within the simulated network environment.

The Network class starts by initializing two distinct ASes, which I refer to as AS1 and AS2, providing a structured approach to simulate complex network scenarios involving multiple AS interactions. Each AS is set up with its unique network configuration, topology, and routing protocols via the TopologyController, which is adept at applying specific network topologies to each AS based on predefined settings or dynamic conditions.

My primary function within this class is to connect AS1 and AS2. This involves linking the two systems at a network level, allowing for traffic to flow between them based on the routing configurations. The connectivity logic ensures that AS gateways are properly interconnected, leveraging the routing protocols defined within each AS to manage the data transfer efficiently.

The Network class provides methods to access each AS, which is useful for monitoring, testing, or further configuration after the initial setup. This level of management and visibility is crucial for network simulation and analysis, offering insights into how different network configurations and topologies impact overall performance and reliability.

Overall, this class serves as the orchestrator for the network simulation, ensuring that all parts of the network are correctly initialized, interconnected, and functioning as intended. This setup allows me to explore various networking theories and practices in a controlled and observable environment.

## Usage of DHCP in Router and PC class

In this project, I've implemented DHCP functionality both in the Router and PC classes to simulate the dynamic allocation of IP addresses within the network, adhering closely to how DHCP operates in real networks.

In the Router class, I've incorporated DHCP to manage IP assignments for connected devices. The Router can act as a DHCP server, meaning it's capable of dynamically distributing IP addresses to nodes within its network. This setup includes methods to enable or disable the DHCP server functionality on demand, which is essential for testing different network configurations or simulating networks with varying DHCP server settings. The Router handles DHCP Discover from PCs by responding with Offers and, upon receiving a DHCP Request, it sends an Acknowledgment back. This process not only assigns an IP but also logs and manages these assignments, maintaining a record that can be saved to or loaded from a file for persistent state management or analysis.

In the PC class, the implementation focuses on initiating and responding to the DHCP process. PCs emit DHCP Discover signals to initiate IP configuration, and upon receiving an offer from a DHCP server, they send a request signal. Finally, when they receive an acknowledgment, they set their IP address accordingly. This mirrors the typical DHCP client functionality, where the PC actively seeks to acquire an IP address at startup or when joining a new network, reflecting the DORA (Discover, Offer, Request, Acknowledge) process used in DHCP communications.

Both classes use signals and slots to communicate DHCP-related events, ensuring that the actions are asynchronous and reflect the event-driven nature of network communications. This setup allows me to simulate a network where multiple nodes dynamically obtain and release IP addresses as they would in a real-world scenario, providing an invaluable tool for studying how DHCP impacts network efficiency and behavior.

-------------------------------------------------------------------------------------------------------------

# Phase 4
## Overview

Phase 4, is the final stage of the project where the network simulation is fully developed and ready for rigorous testing. The focus is on ensuring that the simulator is robust, performs efficiently, and adheres to the defined specifications.


## Features
- Simulator

-----------------------------------------------------------


# Code Documentation
## 1. Simulator
A fully functional network simulation system capable of modeling complex network environments and interactions. کامل کن اگه لازمه

-----------------------------------------------------------

# `Evaluation`
...

## Packet Loss: 


## Average Hops: 


## Buffer Metrics: 


## Network Utilization: 


## Peak Traffic Performance: 


## Router Load: 
