QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp

HEADERS +=

QMAKE_CFLAGS += -fpermissive
QMAKE_CXXFLAGS += -fpermissive
QMAKE_LFLAGS += -fpermissive

INCLUDEPATH += "C:\Program Files\OpenSSL-Win64\include"
LIBS += "C:\Program Files\OpenSSL-Win64\bin\libcrypto-3-x64.dll"
LIBS += "C:\Program Files\OpenSSL-Win64\bin\libssl-3-x64.dll"

# INCLUDEPATH += C:/CppLibs/boost_1_77_0/boost_mingw81_64/include/boost-1_77
INCLUDEPATH += C:/CppLibs/boost_1_78_0
LIBS += -LC:/CppLibs/boost_1_78_0/gcc/lib -llibboost_program_options-mgw8-mt-d-x64-1_78.dll
LIBS += -lws2_32 -lwsock32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
