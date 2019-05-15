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
        private:
            Q_PROPERTY(int version READ getVersion FINAL)
        private:
            Q_PROPERTY(QObject* privateSharedObject READ getPrivateSharedObject FINAL)
        public:
            StaticGlobal();
            ~StaticGlobal();
        public:
            int getVersion() const;
            inline QObject *getPrivateSharedObject() const;
        private:
            sstd_class(StaticGlobal);
        };

        inline QObject *StaticGlobal::getPrivateSharedObject() const {
            return const_cast<StaticGlobal*>(this);
        }

    }/*namespace global*/
}/*namespace sstd*/


















