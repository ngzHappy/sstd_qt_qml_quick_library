#include "sstd_qt_qml_quick_library_application.hpp"
#include <exception>

#define \uacf1_call_if(\uaca1,\uaca2) if constexpr( \uaca1 ){  \uaca2 ; } static_assert(true)

namespace sstd {

    inline constexpr bool isRelease() {
#if defined(QT_DEBUG)||defined(_DEBUG)
        return false;
#else
        return true;
#endif
    }

    QtApplication::QtApplication(int & argc, char ** argv, BeforeAfterQtApplication&&) :
        QApplication(argc, argv),
        Application(argc, argv) {
    }

    /*this function will call before QtApplication construct*/
    BeforeAfterQtApplication::BeforeAfterQtApplication() {
        /*初始化随机种子*/
        \uacf1_call_if(true, ::srand(static_cast<unsigned>(::time(nullptr))));
        /*支持高分屏*/
        \uacf1_call_if(true, QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling));
        /*设置默认OpenGL环境*/
        \uacf1_call_if(true, QSurfaceFormat::setDefaultFormat(sstd::getDefaultQSurfaceFormat()));
        /*开启OpenGL共享资源*/
        \uacf1_call_if(true, QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts));
        /*debug模式关闭异步渲染,release模式开启异步渲染*/
        \uacf1_call_if(!isRelease(), ::qputenv("QSG_RENDER_LOOP", "basic"));
    }

    /*this function will call after QtApplication construct*/
    BeforeAfterQtApplication::~BeforeAfterQtApplication() {
        if (std::uncaught_exceptions() > 0) {
            return;
        }
    }

}/*namespace sstd*/
























