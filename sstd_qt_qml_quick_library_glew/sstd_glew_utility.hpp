#pragma once

#include "../global_sstd_qt_qml_quick_library.hpp"
#include <QtGui/qimage.h>
#include <string_view>

#ifndef _SSTD_HAS_GLEW_0_1
#include <QtGui/qopenglfunctions.h>
#include <QtGui/qopenglfunctions_4_5_core.h>
#endif

namespace sstd::opengl_utility {

    SSTD_QT_SYMBOL_DECL GLuint createTexture(QImage);
    SSTD_QT_SYMBOL_DECL QSize getTextureSize(GLuint);
    SSTD_QT_SYMBOL_DECL void updateTexture(GLuint *, QImage);
    SSTD_QT_SYMBOL_DECL GLuint createVFProgram(std::string_view,std::string_view);

}/*namespace sstd*/
