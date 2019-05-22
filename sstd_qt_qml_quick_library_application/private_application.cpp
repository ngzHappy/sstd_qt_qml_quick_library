#include "private_application.hpp"
#include <QtQml/QtQml>
#include <QtQuick/QtQuick>
#include <QtQuickControls2/QtQuickControls2>
#include "sstd_qt_qml_quick_library_application.hpp"
#include "../sstd_qt_qml_quick_library_path/sstd_qt_qml_quick_library_path.hpp"
#include <cassert>

#if defined(_DEBUG)
#define the_qml "sstd_app_contex/theqml_the_debug/"
#else
#define the_qml "sstd_app_contex/theqml/"
#endif

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

        void StaticGlobal::reloadDefaultStyle() {
            auto varWindow = thisPrivateDefaultWindow.data();
            if (!varWindow) {
                qWarning() << QStringLiteral("do not have main Window ! ");
                return;
            }
            auto varContex = QQmlEngine::contextForObject(varWindow);
            if (!varContex) {
                qWarning() << QStringLiteral("contex is null");
                return;
            }
            auto varEngine = varContex->engine();
            if (!varEngine) {
                qWarning() << QStringLiteral("engine is null");
                return;
            }
            QQmlComponent varComponent{ varEngine };
            const auto varDirPath = qApp->applicationDirPath();
            QDir varDir{ varDirPath };
#define DefaultStyleConfig "sstd_qt_qml_quick_library/DefaultStyleConfig.qml"
            const auto varFullPath =
                varDir.absoluteFilePath(QStringLiteral(the_qml/**/DefaultStyleConfig));
            QFile varFile{ varFullPath };
            if (!varFile.open(QIODevice::ReadOnly)) {
                qWarning() << QStringLiteral("can not open : ") << varFullPath;
                return;
            }
            const auto varUrl = sstd::getLocalFileFullPath(
                QStringLiteral(the_qml/**/DefaultStyleConfig),
                varDirPath);
            varComponent.setData(varFile.readAll(), varUrl);
            auto varObject = varComponent.beginCreate(varContex);
            varComponent.completeCreate();
            try {
                QMetaObject::invokeMethod(varObject, "reloadDefaultStyle", Qt::DirectConnection);
            } catch (...) {
            }
            delete varObject;
        }

        QVariant StaticGlobal::timeSinceCreate() const {
            auto const varNow = std::chrono::steady_clock::now();
            return std::chrono::duration_cast<
                std::chrono::milliseconds>(varNow - thisStart).count();
        }

        void StaticGlobal::privateUpdateTheme() {
            if (thisIsDark) {
                this->setTheme(Dark);
            } else {
                this->setTheme(Light);
            }
        }

        inline static void setWindowTheme(QObject * arg, bool isDark) {
            if (arg->isWindowType()) {
                auto varWindow = qobject_cast<QQuickWindow *>(arg);
                if (varWindow) {
                    auto varSSTDStyle = varWindow->findChild<QObject*>(QStringLiteral("sstdStyled"), Qt::FindDirectChildrenOnly);
                    if (!varSSTDStyle) {
                        auto varContex = QQmlEngine::contextForObject(varWindow);
                        auto varJSEngine = varContex->engine();
                        QQmlComponent varComponent{ varJSEngine , varWindow };
                        varComponent.setData(QByteArrayLiteral(u8R"___(
import QtQuick.Controls.Material 2.12
import QtQuick 2.9
QtObject {
id : idRoot
objectName : "sstdStyled";
property var theMaterial;/*chane here if you changed styled!!!*/
function constructThis( window ){
    theMaterial = window.Material;
}
function setToDark(){
    theMaterial.theme = theMaterial.Dark;
}
function setToLight(){
    theMaterial.theme = theMaterial.Light;
}
}
)___"), {});
                        varSSTDStyle = varComponent.beginCreate(varContex);
                        varSSTDStyle->setParent(varWindow);
                        varComponent.completeCreate();
                        QMetaObject::invokeMethod(varSSTDStyle, "constructThis",
                            Q_ARG(QVariant, QVariant::fromValue(varWindow)));
                    }
                    if (isDark) {
                        QMetaObject::invokeMethod(varSSTDStyle, "setToDark");
                    } else {
                        QMetaObject::invokeMethod(varSSTDStyle, "setToLight");
                    }
                }
            }
        }

        void StaticGlobal::setPrivateDefaultWindow(QQuickWindow * arg) {
            if (thisPrivateDefaultWindow.data() == arg) {
                return;
            }
            if (thisPrivateDefaultWindow.data()) {
                qWarning() << QStringLiteral("can not set again!!");
                return;
            }
            thisPrivateDefaultWindow = arg;
            privateDefaultWindowChanged();
        }

        void StaticGlobal::setIsDark(bool arg) {
            if (arg == thisIsDark) {
                return;
            }
            thisIsDark = arg;
            privateUpdateTheme();
            {
                auto varAllWindows = qApp->topLevelWindows();
                for (auto varI : qAsConst(varAllWindows)) {
                    setWindowTheme(varI, arg);
                }
            }
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

