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

    BeforeAfterQtApplicationControl::BeforeAfterQtApplicationControl(int & argc,
        char ** argv,
        BeforeAfterQtApplication&&args) :
        thisArgC(argc),
        thisArgv(argv) {
        args.construct(argv[0]);
    }

    QtApplication::QtApplication(BeforeAfterQtApplicationControl && arg) :
        QApplication(arg.thisArgC, arg.thisArgv),
        Application(arg.thisArgC, arg.thisArgv) {
    }

    template<typename T>
    inline void remove_utf8_bom(T & argReadStream) {
        std::array varBuffer{ '1','2','3' };
        argReadStream.read(varBuffer.data(), varBuffer.size());
        if (argReadStream.gcount() < 3) {
        } else {
            constexpr std::array varBom{ '\xEF','\xBB','\xBF' };
            if (varBuffer == varBom) {
                return;
            }
        }
        argReadStream.clear();
        argReadStream.seekg(0);
        assert(argReadStream.good());
        return;
    }

    extern int & defaultMultiSampleSize();
    void BeforeAfterQtApplication::construct(const char * argv) {
        do {
            /*读取配置文件，尝试更改multisample值*/
            sstd::filesystem::path varRootPathApplication{ argv };
            sstd::filesystem::path varRootPath = varRootPathApplication.parent_path();
            auto varFileName = varRootPath / "sstd_app_contex"sv / "multisample.txt"sv;
            std::ifstream varStream{ sstd::getFileStreamPath(varFileName),std::ios::binary };
            varStream.sync_with_stdio(false);
            if (varStream.is_open() == false) {
                qWarning() << QStringLiteral("can not find sstd_app_contex/multisample.txt");
                break;
            }
            remove_utf8_bom(varStream);
            int varMultiSamleValue{ -1 };
            varStream >> varMultiSamleValue;
            defaultMultiSampleSize() = varMultiSamleValue;
            assert(varMultiSamleValue < 256);
            assert(varMultiSamleValue > -256);
        } while (false);
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
        /*debug模式开始import调试输出*/
        \uacf1_call_if(!isRelease(), ::qputenv("QML_IMPORT_TRACE", QByteArrayLiteral("1")));
    }

    /*this function will call before QtApplication construct*/
    BeforeAfterQtApplication::BeforeAfterQtApplication(QString argStyle) {
        /*设置样式*/
        if (!argStyle.isEmpty()) {
            const auto varStypeFileName = argStyle.toLocal8Bit();
            ::qputenv("QT_QUICK_CONTROLS_CONF", varStypeFileName);
        }
    }

    namespace {
        class OpenGLConstruct {
        public:
            QOffscreenSurface surface;
            std::optional< QOpenGLContext > contex;
            QOpenGLContext * globalContex{ nullptr };
            inline OpenGLConstruct() {
                surface.setFormat(sstd::getDefaultQSurfaceFormat());
                surface.create();
                if (!QOpenGLContext::globalShareContext()) {
                    contex.emplace();
                    contex->setFormat(sstd::getDefaultQSurfaceFormat());
                    contex->create();
                    contex->makeCurrent(&surface);
                } else {
                    globalContex = QOpenGLContext::globalShareContext();
                    globalContex->makeCurrent(&surface);
                }
                if (false == sstd::glew_initialize()) {
                    qFatal("can not init glew!!!");
                }
            }
        private:
            sstd_class(OpenGLConstruct);
        };
    }

    OpenGLConstruct * globalOpenGlContex{ nullptr };

    QOpenGLContext * QtApplication::getGlobalSharedContex() {
        if (globalOpenGlContex->globalContex) {
            return globalOpenGlContex->globalContex;
        }
        return &(*(globalOpenGlContex->contex));
    }

    /*this function will call after QtApplication construct*/
    BeforeAfterQtApplication::~BeforeAfterQtApplication() {
        if (std::uncaught_exceptions() > 0) {
            return;
        }
        {
            assert(globalOpenGlContex == nullptr);
            globalOpenGlContex = sstd_new<OpenGLConstruct>();
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
