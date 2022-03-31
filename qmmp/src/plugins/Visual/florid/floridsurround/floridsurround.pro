include($$PWD/../../Visual.pri)

DESTDIR = $$PLUGINS_PREFIX/Visual
TARGET = $${TARGET}

HEADERS += floridsurround.h \
           visualfloridsurroundfactory.h
           
SOURCES += floridsurround.cpp \
           visualfloridsurroundfactory.cpp

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
}
