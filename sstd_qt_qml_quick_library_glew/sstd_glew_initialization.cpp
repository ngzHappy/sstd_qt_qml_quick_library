#include "sstd_glew_initialization.hpp"

#include "GLSSTD/glew.h"
#include <sstd_library.hpp>
#include <string_view>
#include <mutex>
#include <QtCore/QtCore>

using namespace std::string_view_literals;

inline static bool _0_sstd_init_glew() {

    glewExperimental = GL_TRUE;
    auto varGlewError = glewInit();
    if (varGlewError != GLEW_OK) {
        qFatal( "glew init error!") ;
        return false;
    }

    return true;

}

inline static bool _call_once_init_glew() {
    static std::once_flag varCallOnce;
    static bool varAns{ true };
    std::call_once(varCallOnce, [](bool * ans) {
        *ans = _0_sstd_init_glew();
    }, &varAns);
    return varAns;
}

SSTD_QT_SYMBOL_DECL bool sstd::glew_initialize() {
    return _call_once_init_glew();
}
