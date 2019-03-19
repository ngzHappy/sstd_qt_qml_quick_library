
TEMPLATE = lib

DEFINES *= SSTD_QT_BUILD_SOURCE

CONFIG(debug,debug|release){
    TARGET = sstd_qt_qml_quick_library_debug
}else{
    TARGET = sstd_qt_qml_quick_library
}

include($$PWD/used_qt_library.pri)
include($$PWD/../sstd_library/sstd_library.pri)

DESTDIR = $${SSTD_LIBRARY_OUTPUT_PATH}

HEADERS += $$PWD/sstd_qt_qml_quick_library.hpp
HEADERS += $$PWD/global_sstd_qt_qml_quick_library.hpp

SOURCES += $$PWD/sstd_qt_qml_quick_library_thread/sstd_qt_qml_quick_library_thread.cpp
HEADERS += $$PWD/sstd_qt_qml_quick_library_thread/sstd_qt_qml_quick_library_thread.hpp
