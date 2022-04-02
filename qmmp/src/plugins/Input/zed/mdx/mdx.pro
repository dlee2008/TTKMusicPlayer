include($$PWD/../zed.pri)

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = $${TARGET}

HEADERS += decodermdxfactory.h \
           decoder_mdx.h \
           mdxhelper.h
    
SOURCES += decodermdxfactory.cpp \
           decoder_mdx.cpp \
           mdxhelper.cpp

INCLUDEPATH += $$EXTRA_PREFIX/libzed/include

unix{
    QMAKE_CLEAN = $$DESTDIR/lib$${TARGET}.so
    LIBS += -L$$EXTRA_PREFIX/libzed/lib -lmdx$$STATIC_LIBRARY_SUFFIX
}

win32{
    LIBS += -L$$EXTRA_PREFIX/libzed/lib -lmdx
}
