#pragma once
#include "../global_sstd_qt_qml_quick_library.hpp"

namespace sstd {

    SSTD_QT_SYMBOL_DECL bool glew_initialize();

}/*namespace sstd*/

#if defined(SSTD_HAS_GLEW_APPLICATION_INITIALIZE)

#ifndef sstd_glew_initialize
#define sstd_glew_initialize() static_assert(true)
#endif

#else

#ifndef sstd_glew_initialize
#define sstd_glew_initialize sstd::glew_initialize
#endif

#endif
