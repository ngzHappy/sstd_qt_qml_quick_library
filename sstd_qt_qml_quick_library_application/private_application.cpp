#include "private_application.hpp"
#include <QtQml/QtQml>
#include <QtQuick/QtQuick>
#include <cassert>

namespace sstd {
    namespace global {

        StaticGlobalContex::StaticGlobalContex(QQmlEngine *engine, QObject *parent ) :
            Super(engine,parent) {
        }

        StaticGlobalContex::~StaticGlobalContex() {
        }

        StaticGlobal::StaticGlobal(){
        }

        StaticGlobal::~StaticGlobal() {
            /*this shoule never call*/
            assert(false);
        }

        int StaticGlobal::getVersion() const{
            return 1;
        }

    }
}

inline static void registerThis(){
    qmlRegisterSingletonType<sstd::global::StaticGlobal>("sstd.styled.app", 1, 0,
            "GlobalAppData",
            [](QQmlEngine *engine, QJSEngine * ) -> QObject *{
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
        static auto globalObject = sstd_new<sstd::global::StaticGlobal>();
        auto varContex = sstd_new<sstd::global::StaticGlobalContex>(engine,engine);
        varContex->setContextObject(globalObject);
        auto varAns = varComponent.beginCreate(varContex);
        varAns->setParent( engine );
        varComponent.completeCreate();
        return varAns;
    });
}

Q_COREAPP_STARTUP_FUNCTION(registerThis)

