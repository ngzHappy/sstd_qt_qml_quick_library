#include "private_application.hpp"
#include <QtQml/QtQml>
#include <QtQuick/QtQuick>
#include <QtQuickControls2/QtQuickControls2>
#include "sstd_qt_qml_quick_library_application.hpp"
#include <cassert>

namespace sstd {

    namespace global {

        inline QString fallBackStyle() {
            return QStringLiteral("Material");
        }

#if defined(_DEBUG)
        bool isQtStyledApplication{ false };
#endif
    }/* namespace global */

    QtStyledApplication::QtStyledApplication(int & argc, char ** argv) :
        QtApplication(argc, argv) {
        QQuickStyle::setStyle(global::fallBackStyle());
        QQuickStyle::setFallbackStyle(global::fallBackStyle());
#if defined(_DEBUG)
        global::isQtStyledApplication = true;
#endif
    }

    namespace global {

        StaticGlobal::StaticGlobal() {
            QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
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

        namespace dark {

            inline QColor globalGetAccent() {
                return QColor(0x0f4, 0x08f, 0x0b1);
            }

            inline QColor globalGetBackground() {
                return QColor(0x030, 0x030, 0x030);
            }

            inline QColor globalGetForeground() {
                return QColor(0x0ff, 0x0ff, 0x0ff);
            }

            inline QColor globalGetPrimary() {
                return QColor(0x09f, 0x0a8, 0x0da);
            }

        }/*namespace drak*/

        namespace light {

            inline QColor globalGetAccent() {
                return QColor(0x0e9, 0x01e, 0x063);
            }

            inline QColor globalGetBackground() {
                return QColor(0x0fa, 0x0fa, 0x0fa);
            }

            inline QColor globalGetForeground() {
                return QColor(0x000, 0x000, 0x000);
            }

            inline QColor globalGetPrimary() {
                return QColor(0x03f, 0x051, 0x0b5);
            }

        }/*namespace light*/

        int StaticGlobal::getElevation() const {
            return globalGetElevation();
        }

        QColor StaticGlobal::getAccent() const {
            return isDark() ? dark::globalGetAccent() : light::globalGetAccent();
        }

        QColor StaticGlobal::getBackground() const {
            return isDark() ? dark::globalGetBackground() : light::globalGetBackground();
        }

        QColor StaticGlobal::getForeground() const {
            return isDark() ? dark::globalGetForeground() : light::globalGetForeground();
        }

        QColor StaticGlobal::getPrimary() const {
            return isDark() ? dark::globalGetPrimary() : light::globalGetPrimary();
        }

        bool StaticGlobal::isDark() const {
            return thisIsDark;
        }

        void StaticGlobal::setIsDark( bool arg) {
            if (arg == thisIsDark) {
                return;
            }
            thisIsDark = arg;
            accentChanged();
            backgroundChanged();
            foregroundChanged();
            primaryChanged();
            isDarkChanged();
        }

        inline auto instanceStaticGlobal() {
            static auto globalAns = sstd_new<StaticGlobal>();
            return globalAns;
        }

    }
}

inline static void registerThis() {
    using namespace sstd::global;
    const char * globalURI = "sstd.styled.app";

    qmlRegisterSingletonType< StaticGlobal >(globalURI, 1, 0, "GlobalAppData",
        [](QQmlEngine *, QJSEngine *)->QObject * {
#if defined(_DEBUG)
        assert(isQtStyledApplication);
#endif
        return instanceStaticGlobal();
    });
}

Q_COREAPP_STARTUP_FUNCTION(registerThis)

