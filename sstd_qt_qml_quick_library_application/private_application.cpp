#include "private_application.hpp"
#include <QtQml/QtQml>
#include <QtQuick/QtQuick>
#include <QtQuickControls2/QtQuickControls2>
#include "sstd_qt_qml_quick_library_application.hpp"
#include <cassert>

namespace sstd {

    inline QString fallBackStyle() {
        return QStringLiteral("Material");
    }

    QtStyledApplication::QtStyledApplication(int & argc, char ** argv) :
        QtApplication(argc, argv) {
        QQuickStyle::setStyle(fallBackStyle());
        QQuickStyle::setFallbackStyle(fallBackStyle());
    }

    namespace global {

        StaticGlobalContex::StaticGlobalContex(QQmlEngine *engine, QObject *parent) :
            Super(engine, parent) {
        }

        StaticGlobalContex::~StaticGlobalContex() {
        }

        StaticGlobal::StaticGlobal() {
        }

        StaticGlobal::~StaticGlobal() {
            /*this shoule never call*/
            assert(false);
        }

        int StaticGlobal::getVersion() const {
            return 1;
        }

    }
}

inline static void registerThis() {
    qmlRegisterSingletonType<sstd::global::StaticGlobal>("sstd.styled.app", 1, 0,
        "GlobalAppData",
        [](QQmlEngine *engine, QJSEngine *) -> QObject * {
        QQmlComponent varComponent{ engine };
        varComponent.setData(QByteArrayLiteral(u8R"(

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

QtObject {
    property var appStyle : Material ;
}

)"), QUrl{});
        assert(engine);
        assert(fallBackStyle() == QQuickStyle::setFallbackStyle());
        static auto globalObject = sstd_new<sstd::global::StaticGlobal>();
        auto varContex = sstd_new<sstd::global::StaticGlobalContex>(engine, engine);
        varContex->setContextObject(globalObject);
        auto varAns = varComponent.beginCreate(varContex);
        varComponent.completeCreate();
        return varAns;
    });
}

Q_COREAPP_STARTUP_FUNCTION(registerThis)

