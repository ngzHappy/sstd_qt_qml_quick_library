
include($$PWD/used_qt_library.pri)
include($$PWD/../sstd_library/sstd_build_path.pri)

#import
INCLUDEPATH += $$PWD
CONFIG(debug,debug|release){
    LIBS += -L$${SSTD_LIBRARY_OUTPUT_PATH} -lsstd_qt_qml_quick_library_debug
}else{
    LIBS += -L$${SSTD_LIBRARY_OUTPUT_PATH} -lsstd_qt_qml_quick_library
}





