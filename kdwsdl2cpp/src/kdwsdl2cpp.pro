TEMPLATE = app
TARGET = kdwsdl2cpp
SOURCES = main.cpp
INCLUDEPATH += $${TOP_SOURCE_DIR}/kdwsdl2cpp
QT -= gui
QT += xml

CONFIG += console
macx:CONFIG -= app_bundle
static { macx:QMAKE_LFLAGS += -Wl,-rpath,$$[QT_INSTALL_LIBS] }

# Relink when a static lib changed
unix:PRE_TARGETDEPS += $${TOP_BUILD_DIR}/lib/libkode.a \
    $${TOP_BUILD_DIR}/lib/libwsdl.a \
    $${TOP_BUILD_DIR}/lib/libxmlschema.a \
    $${TOP_BUILD_DIR}/lib/libxmlcommon.a \
    $${TOP_BUILD_DIR}/lib/libkdwsdl2cpp_lib.a

win32-msvc*:PRE_TARGETDEPS += $${TOP_BUILD_DIR}/lib/kode$$DEBUG_SUFFIX.lib \
    $${TOP_BUILD_DIR}/lib/wsdl$$DEBUG_SUFFIX.lib \
    $${TOP_BUILD_DIR}/lib/xmlschema$$DEBUG_SUFFIX.lib \
    $${TOP_BUILD_DIR}/lib/xmlcommon$$DEBUG_SUFFIX.lib \
    $${TOP_BUILD_DIR}/lib/kdwsdl2cpp_lib$$DEBUG_SUFFIX.lib

LIBS += -L$${TOP_BUILD_DIR}/lib \
    -l$${KDWSDL2CPP_LIB} \
    -lkode \
    -lwsdl \
    -lxmlschema \
    -lxmlcommon

include($${TOP_SOURCE_DIR}/variables.pri)
DEFINES -= QT_NO_CAST_TO_ASCII \
    QBA_NO_CAST_TO_VOID \
    QBA_NO_CAST_TO_ASCII \
    QT_NO_CAST_FROM_ASCII

#installation targets
target.path = $$INSTALL_PREFIX/bin

INSTALLS += target