
TEMPLATE = app

CONFIG += console

CONFIG(debug,debug|release){
    TARGET = sstd_qt_qml_quick_library_test_debug
}else{
    TARGET = sstd_qt_qml_quick_library_test
}

include($$PWD/../../sstd_library/sstd_library.pri)
include($$PWD/../sstd_qt_qml_quick_library.pri)

DESTDIR = $${SSTD_LIBRARY_OUTPUT_PATH}

SOURCES += $$PWD/main.cpp




