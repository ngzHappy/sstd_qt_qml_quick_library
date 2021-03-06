﻿#include "sstd_qt_qml_quick_library_opengl_default_format.hpp"

#ifdef  _DEBUG

#define ENABLE_GL_DEBUG (1u)

#else

#endif /*_DEBUG*/

namespace sstd {

    extern int & defaultMultiSampleSize(){
        static int varAns{ 4 };
        return varAns;
    }

    namespace {
        inline QSurfaceFormat _0_GetDefaultQSurfaceFormat() {
            auto varFormat = QSurfaceFormat::defaultFormat();
            varFormat.setVersion(4, 6);
            varFormat.setProfile(QSurfaceFormat::CoreProfile);
            varFormat.setAlphaBufferSize(8);
            varFormat.setBlueBufferSize(8);
            varFormat.setRedBufferSize(8);
            varFormat.setGreenBufferSize(8);
            varFormat.setDepthBufferSize(24);
            varFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
            varFormat.setRenderableType(QSurfaceFormat::OpenGL);
            varFormat.setSwapInterval(0)/*关闭垂直同步*/;
            varFormat.setSamples(defaultMultiSampleSize());
#if defined(ENABLE_GL_DEBUG)
            varFormat.setOption(QSurfaceFormat::DebugContext, true);
#else
            varFormat.setOption(QSurfaceFormat::DebugContext, false);
#endif
            return varFormat;
        }
    }/*namespace*/

    SSTD_QT_SYMBOL_DECL const QSurfaceFormat & getDefaultQSurfaceFormat() {
        const static QSurfaceFormat varAns = _0_GetDefaultQSurfaceFormat();
        return varAns;
    }

}/*namespace sstd*/
