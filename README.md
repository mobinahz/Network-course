# Computer Networks Course Projects

This repository contains three independent projects developed for a Computer Networks course. Each project has its own source tree, Qt project file, documentation, and build process; there is no single application or root-level build.

## Projects

| Directory | Project | Summary | Main technologies |
| --- | --- | --- | --- |
| [`project-1/`](project-1/) | Distributed voice call | Desktop audio calling prototype. Peers exchange WebRTC signaling messages through a WebSocket relay; audio is captured and encoded with Opus. | C++, Qt 6, WebRTC/libdatachannel, Opus, Node.js, WebSocket |
| [`project-3/`](project-3/) | Network protocol simulator | Simulates generated packet traffic across network nodes, with IPv4/IPv6, DHCP, routing, and packet/header components. | C++, Qt, qmake |
| [`project-4/`](project-4/) | Network simulator GUI | Interactive network simulation with configurable topology and timing, routing, packet delivery, and TCP congestion-control behavior. | C++, Qt Widgets, qmake |

The directory numbering reflects the original coursework layout; **there is no `project-2/` directory in this repository**.

## Requirements

- Qt and its matching development tools (`qmake` or Qt Creator). Use a Qt kit compatible with the modules and compiler required by the project.
- A C++ compiler supported by that Qt kit.
- For Project 1, Node.js/npm plus the native dependencies used by the client: libdatachannel, Opus, and the required TLS libraries.

Project 3 and Project 4 use different Qt/C++ configurations. Open each project's `.pro` file separately in Qt Creator and select a suitable kit. The existing `build/` directories contain generated build artifacts and should not be treated as portable build instructions.

## Build and Run

### Project 1: Voice Call

Start the signaling relay from the repository root:

```sh
cd project-1/signaling-server
npm install
node server.js
```

The relay listens on `ws://localhost:3000`. Build the client by opening [`project-1/CA1.pro`](project-1/CA1.pro) in Qt Creator, configuring the native dependencies, and building with a compatible Qt kit.

**Platform setup:** the current qmake project file contains absolute Windows paths for libdatachannel, Opus, and OpenSSL. These paths must be changed to match your local installation before the client can build; the checked-in project is not configured for a clean macOS/Linux build. The relay broadcasts messages to every other connected client and does not provide authentication or separate call rooms, so use it only in a trusted development environment.

### Project 3: Protocol Simulator

Open [`project-3/CN_CA_3.pro`](project-3/CN_CA_3.pro) in Qt Creator, configure a Qt kit, and build. The qmake project includes the implementation through [`project-3/src/src.pri`](project-3/src/src.pri). The project entry point creates and starts a simulator using `QCoreApplication`.

### Project 4: Network Simulator GUI

Open [`project-4/CN_CA3.pro`](project-4/CN_CA3.pro) in Qt Creator, configure a Qt Widgets kit, and build. Run the resulting application from the `project-4/` directory so that any relative paths to the included simulation resources resolve as expected.

## Documentation

- [Project 1 README](project-1/README.md): voice-call design, signaling, audio components, and WebRTC notes.
- [Project 3 README](project-3/README.md): project landing page; see its [reports](project-3/) for phase details.
- [Project 3 report](project-3/Report-CA3.md)
- [Project 3 phase 2 report](project-3/phase%202%20Report.md)
- [Project 4 README](project-4/README.md): simulator and TCP behavior overview.

## Tests

No repository-wide test or build command is configured. The signaling-server `package.json` does not define a test suite. Build and validate each project independently with its selected Qt kit and any project-specific native dependencies.