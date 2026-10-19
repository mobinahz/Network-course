QT       += core gui quick qml core multimedia websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    src/audio/audioinput.cpp \
    src/audio/audiooutput.cpp \
    src/controller/Controller.cpp \
    src/controller/audiocontroller.cpp \
    src/main.cpp \
    src/network/peer.cpp \
    src/network/webrtc.cpp


HEADERS += \
    src/audio/audioinput.h \
    src/audio/audiooutput.h \
    src/controller/Controller.h \
    src/controller/audiocontroller.h \
    src/network/json.hpp \
    src/network/peer.h \
    src/network/webrtc.h



FORMS +=


DEFINES += _WEBSOCKETPP_CPP11_STL_
DEFINES += _WEBSOCKETPP_CPP11_FUNCTIONAL_
DEFINES += SIO_TLS
DEFINES += BOOST_DATE_TIME_NO_LIB
DEFINES += BOOST_REGEX_NO_LIB
DEFINES += ASIO_STANDALONE

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

PATH_TO_LIBDATACHANNEL = "C:/Users/Asus/libdatachannel"

INCLUDEPATH += $$PATH_TO_LIBDATACHANNEL/include
LIBS += -L$$PATH_TO_LIBDATACHANNEL/Windows/Mingw64 -ldatachannel.dll
LIBS += -LC:/Qt/Tools/OpenSSLv3/Win_x64/bin -lcrypto-3-x64 -lssl-3-x64
LIBS += -lws2_32
LIBS += -lssp

PATH_TO_OPUS = "C:/Users/Asus/opus"

INCLUDEPATH += $$PATH_TO_OPUS/include
LIBS += -L$$PATH_TO_OPUS/Windows/Mingw64 -lopus


DISTFILES += \
    signaling-server/server.js \
    src/ui/Main.qml
