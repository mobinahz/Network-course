Computer Networks - CA3 - Phase1

Arshiya Ataei - Mobina Haghizadeh

810100252 - 810100127
--------------------------------------------------------------
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
