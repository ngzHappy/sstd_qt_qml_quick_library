#include "private_application.hpp"
#include <QtQml/QtQml>
#include <QtQuick/QtQuick>
#include <QtQuickControls2/QtQuickControls2>
#include "sstd_qt_qml_quick_library_application.hpp"
#include "../sstd_qt_qml_quick_library_path/sstd_qt_qml_quick_library_path.hpp"
#include <cassert>

inline static void registerThis(QCoreApplication *);

namespace sstd {

    namespace global {

        inline QString fallBackStyle() {
            return QStringLiteral("Material");
        }

#if defined(_DEBUG)
        inline bool static isQtStyledApplication{ false };
#endif
    }/* namespace global */

    QtStyledApplication::QtStyledApplication(int & argc, char ** argv) :
        QtApplication(argc, argv) {
        QQuickStyle::setStyle(global::fallBackStyle());
        QQuickStyle::setFallbackStyle(global::fallBackStyle());
#if defined(_DEBUG)
        global::isQtStyledApplication = true;
#endif
        registerThis(this);
    }

    namespace global {

        inline auto whenCreate() {
            const auto static globalAns = std::chrono::steady_clock::now();
            return globalAns;
        }

        StaticGlobal::StaticGlobal() : thisStart{ whenCreate() } {
            QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
            privateUpdateTheme();
        }

        StaticGlobal::~StaticGlobal() {
            /*this shoule never call*/
            assert(false);
        }

        int StaticGlobal::getVersion() const {
            return 1;
        }

        /*data copy from : qquickmaterialstyle.cpp */
        inline int globalGetElevation() {
            return 0;
        }

        qlonglong StaticGlobal::timeSinceCreate() const {
            auto const varNow = std::chrono::steady_clock::now();
            return std::chrono::duration_cast<
                std::chrono::milliseconds>(varNow - thisStart).count();
        }

        void StaticGlobal::privateUpdateTheme(){
            if( thisIsDark ){
                this->setTheme( Dark );
            }else{
                this->setTheme( Light );
            }
        }

        void StaticGlobal::setIsDark(bool arg) {
            if (arg == thisIsDark) {
                return;
            }
            thisIsDark = arg;
            privateUpdateTheme();
            isDarkChanged();
        }

        inline auto instanceStaticGlobal() {
            static auto globalAns = sstd_new<StaticGlobal>();
            return globalAns;
        }

    }
}

/* import sstd.styled.app 1.0 //GlobalAppData */
inline static void registerThis(QCoreApplication *arg) {
    const auto varDirPath = arg->applicationDirPath();
#if defined(_DEBUG)
#define the_qml "sstd_app_contex/theqml_the_debug/"
#else
#define the_qml "sstd_app_contex/theqml/"
#endif
    constexpr const char * globalURI = "sstd.styled.app";
    qmlRegisterType(sstd::getLocalFileFullPath(
        QStringLiteral(the_qml/**/"sstd_qt_qml_quick_library/StyledApplicationWindow.qml"),
        varDirPath),
        globalURI, 1, 0, "StyledApplicationWindow");
    qmlRegisterSingletonType< sstd::global::StaticGlobal >(globalURI, 1, 0, "GlobalAppData",
        [](QQmlEngine *, QJSEngine *)->QObject * {
#if defined(_DEBUG)
        assert(sstd::global::isQtStyledApplication);
#endif
        return sstd::global::instanceStaticGlobal();
    });
}

