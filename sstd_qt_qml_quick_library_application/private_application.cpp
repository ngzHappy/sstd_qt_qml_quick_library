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
        }

        StaticGlobal::~StaticGlobal() {
            /*this shoule never call*/
            assert(false);
        }

        int StaticGlobal::getVersion() const {
            return 1;
        }

        Static::Static(StaticGlobal * a, QObject * b) :
            thisSharedObject{ a },
            thisAppStyle{ b } {
        }

        Static::~Static() {
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
    qmlRegisterUncreatableType< StaticGlobal >(globalURI, 1, 0,
        "StaticGlobal","can not create StaticGlobal");
    qmlRegisterSingletonType< Static >(globalURI, 1, 0, "GlobalAppData",
        [](QQmlEngine *engine, QJSEngine *)->QObject * {

#if defined(_DEBUG)
        assert(engine);
        assert(sstd::global::isQtStyledApplication);
#endif

        QQmlComponent varComponent{ engine };
        varComponent.setData(QByteArrayLiteral(u8R"(

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

Pane {
    // property var xxx : ApplicationWindow.window.Material ;
    function getStyle() {
 
        return ApplicationWindow.window.appStyle ;
    }
}

)"), QUrl{});

        auto varContexObject = varComponent.beginCreate(engine->rootContext());
        varComponent.completeCreate();

        auto varAns = sstd_new<Static>(instanceStaticGlobal(), varContexObject);
        varContexObject->setParent(varAns);

        return varAns;

    });
}

Q_COREAPP_STARTUP_FUNCTION(registerThis)

