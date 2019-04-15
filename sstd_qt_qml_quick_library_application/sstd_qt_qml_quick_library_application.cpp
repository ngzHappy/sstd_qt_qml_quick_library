#include "sstd_qt_qml_quick_library_application.hpp"
#include "../sstd_qt_qml_quick_library_glew/sstd_glew_initialization.hpp"
#include <QtCore/QtCore>
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
    BeforeAfterQtApplication::BeforeAfterQtApplication(QString argStyle) {
        /*初始化随机种子*/
        \uacf1_call_if(true, ::srand(static_cast<unsigned>(::time(nullptr))));
        /*支持高分屏*/
        \uacf1_call_if(true, QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling));
        /*设置默认OpenGL环境*/
        \uacf1_call_if(true, QSurfaceFormat::setDefaultFormat(sstd::getDefaultQSurfaceFormat()));
        /*开启OpenGL共享资源*/
        \uacf1_call_if(true, QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts));
        /*debug模式关闭异步渲染,release模式开启异步渲染*/
        \uacf1_call_if(!isRelease(), ::qputenv("QSG_RENDER_LOOP", QByteArrayLiteral("basic")));
        /*设置样式*/
        if(!argStyle.isEmpty()){
            const auto varStypeFileName = argStyle.toLocal8Bit();
            ::qputenv("QT_QUICK_CONTROLS_CONF",varStypeFileName );
        }
    }

    namespace {
        class OpenGLConstruct {
        public:
            QOffscreenSurface surface;
            QOpenGLContext contex;
            inline OpenGLConstruct() {
                surface.setFormat(sstd::getDefaultQSurfaceFormat());
                surface.create();
                contex.setFormat(sstd::getDefaultQSurfaceFormat());
                contex.create();
                contex.makeCurrent(&surface);
                if (false == sstd::glew_initialize()) {
                    qFatal("can not init glew!!!");
                }
            }
        private:
            sstd_class(OpenGLConstruct);
        };
    }

    /*this function will call after QtApplication construct*/
    BeforeAfterQtApplication::~BeforeAfterQtApplication() {
        if (std::uncaught_exceptions() > 0) {
            return;
        }
        {
            static auto varOpenGLContex = sstd_new<OpenGLConstruct>();
            (void)varOpenGLContex;
        }
        {
            /*强制加载QImage插件
            防止第一次加载速度过慢*/
            QImage varImage{ QStringLiteral(":/qtandqmlglobal/image/foreceLoadQImage.png") };
            (void)varImage;
        }
        if constexpr (!isRelease()) {
            /*设置Application Name*/
            auto varName = qApp->applicationName();
            const auto varAboutToRemove = QStringLiteral("_debug");
            if (varName.endsWith(varAboutToRemove)) {
                varName.chop(varAboutToRemove.size());
                qApp->setApplicationName(varName);
            }
        }
    }

}/*namespace sstd*/
