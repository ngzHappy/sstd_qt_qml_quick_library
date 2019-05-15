#pragma once

#include <sstd_library.hpp>
#include <QtCore/qobject.h>
#include <QtQml/qqmlcontext.h>

namespace sstd {
    namespace global {

        /*used for styled application*/
        class StaticGlobalContex : public QQmlContext {
            Q_OBJECT
        public:
            StaticGlobalContex(QQmlEngine *engine, QObject *parent = nullptr);
            ~StaticGlobalContex();
        private:
            using Super = QQmlContext;
        private:
            sstd_class(StaticGlobalContex);
        };

        class StaticGlobal : public QObject {
            Q_OBJECT
        public:
            StaticGlobal();
            ~StaticGlobal();
        private:
            sstd_class(StaticGlobal);
        };

    }/*namespace global*/
}/*namespace sstd*/


















