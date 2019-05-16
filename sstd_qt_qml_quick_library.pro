
TEMPLATE = lib

QT += gui_private
QT += core_private

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

SOURCES += $$PWD/sstd_qt_qml_quick_library_gzip/sstd_qt_qml_quick_library_gzip.cpp
HEADERS += $$PWD/sstd_qt_qml_quick_library_gzip/sstd_qt_qml_quick_library_gzip.hpp

SOURCES += $$PWD/sstd_qt_qml_quick_library_application/sstd_qt_qml_quick_library_application.cpp
HEADERS += $$PWD/sstd_qt_qml_quick_library_application/sstd_qt_qml_quick_library_application.hpp

SOURCES += $$PWD/sstd_qt_qml_quick_library_application/private_application.cpp
HEADERS += $$PWD/sstd_qt_qml_quick_library_application/private_application.hpp

SOURCES += $$PWD/sstd_qt_qml_quick_library_opengl_default_format/sstd_qt_qml_quick_library_opengl_default_format.cpp
HEADERS += $$PWD/sstd_qt_qml_quick_library_opengl_default_format/sstd_qt_qml_quick_library_opengl_default_format.hpp

SOURCES += $$PWD/sstd_qt_qml_quick_library_path/sstd_qt_qml_quick_library_path.cpp
HEADERS += $$PWD/sstd_qt_qml_quick_library_path/sstd_qt_qml_quick_library_path.hpp

###################################################################################
#glew
INCLUDEPATH += $$PWD/sstd_qt_qml_quick_library_glew
SOURCES += $$PWD/sstd_qt_qml_quick_library_glew/glew.c
SOURCES += $$PWD/sstd_qt_qml_quick_library_glew/sstd_glew.cpp
SOURCES += $$PWD/sstd_qt_qml_quick_library_glew/sstd_glew_initialization.cpp
SOURCES += $$PWD/sstd_qt_qml_quick_library_glew/sstd_glew_utility.cpp
DEFINES *= GLEW_NO_GLU
DEFINES *= GLEW_STATIC
win32:{
    LIBS+= -lopengl32 -lgdi32 -luser32 -lkernel32
} else {
###libglu1-mesa-dev
###libgl1-mesa-dev
###libxi-dev
###libxmu-dev
    LIBS+=-lXmu -lXi -lGL -lXext -lX11
}
###################################################################################

QMLSOURCES += $$PWD/theqml_the_debug/sstd_qt_qml_quick_library/StyledApplicationWindow.qml

lupdate_only{
    SOURCES += $$QMLSOURCES
}

###################################################################################
RESOURCES += $$PWD/resource/sstd_qt_and_qml_library.qrc

#remove some build warning
win32-msvc*{
    QMAKE_CXXFLAGS += /wd"4251"
}else{
    QMAKE_CXXFLAGS += -Wno-unused-parameter
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations
}

isEmpty(QMAKE_POST_LINK){
    QMAKE_POST_LINK += $${SSTD_LIBRARY_OUTPUT_PATH}/sstd_copy_qml $${PWD} $${PWD} skip
}else{
    QMAKE_POST_LINK += $$escape_expand(\\n\\t)$${SSTD_LIBRARY_OUTPUT_PATH}/sstd_copy_qml $${PWD} $${PWD} skip
}

#all application config
!exists($${DESTDIR}/sstd_app_contex){
    QMAKE_POST_LINK += $$escape_expand(\\n\\t)$${DESTDIR}/sstd_copy_qml $${PWD}/sstd_app_contex $${DESTDIR}/sstd_app_contex release
}

#the library qml files
CONFIG(debug,debug|release) {
    QMAKE_POST_LINK += $$escape_expand(\\n\\t)$${SSTD_LIBRARY_OUTPUT_PATH}/sstd_copy_qml $${PWD}/theqml_the_debug $${SSTD_LIBRARY_OUTPUT_PATH}/theqml_the_debug debug
}else{
    QMAKE_POST_LINK += $$escape_expand(\\n\\t)$${SSTD_LIBRARY_OUTPUT_PATH}/sstd_copy_qml $${PWD}/theqml_the_debug $${SSTD_LIBRARY_OUTPUT_PATH}/theqml release
}
export(QMAKE_POST_LINK)

