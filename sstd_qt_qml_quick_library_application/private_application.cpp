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
    }

    namespace global {

        inline auto whenCreate(){
            const auto varAns =  std::chrono::steady_clock::now();
            return varAns;
        }

        StaticGlobal::StaticGlobal() : thisStart{ whenCreate() } {
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
                return qRgb(0x0'f4, 0x0'8f, 0x0'b1);
            }

            inline QColor globalGetBackground() {
                return qRgb(0x0'30, 0x0'30, 0x0'30);
            }

            inline QColor globalGetForeground() {
                return qRgb(0x0'ff, 0x0'ff, 0x0'ff);
            }

            inline QColor globalGetPrimary() {
                return qRgb(0x0'9f, 0x0'a8, 0x0'da);
            }

        }/*namespace drak*/

        namespace light {

            inline QColor globalGetAccent() {
                return qRgb(0x0'e9, 0x0'1e, 0x0'63);
            }

            inline QColor globalGetBackground() {
                return qRgb(0x0'fa, 0x0'fa, 0x0'fa);
            }

            inline QColor globalGetForeground() {
                return qRgb(0x0'00, 0x0'00, 0x0'00);
            }

            inline QColor globalGetPrimary() {
                return qRgb(0x0'3f, 0x0'51, 0x0'b5);
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

        qlonglong StaticGlobal::timeSinceCreate() const{
            auto const varNow = std::chrono::steady_clock::now();
            return std::chrono::duration_cast<
                    std::chrono::milliseconds>(varNow - thisStart).count();
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

/* import sstd.styled.app 1.0 //GlobalAppData */
inline static void registerThis() {
    qmlRegisterSingletonType< sstd::global::StaticGlobal >("sstd.styled.app", 1, 0, "GlobalAppData",
        [](QQmlEngine *, QJSEngine *)->QObject * {
#if defined(_DEBUG)
        assert(sstd::global::isQtStyledApplication);
#endif
        return sstd::global::instanceStaticGlobal();
    });
}

Q_COREAPP_STARTUP_FUNCTION(registerThis)

