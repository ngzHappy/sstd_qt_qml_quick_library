#pragma once

#include <sstd_library.hpp>
#include <QtCore/qobject.h>
#include <QtQml/qqmlcontext.h>

namespace sstd {
    namespace global {

        class StaticGlobal : public QObject {
            Q_OBJECT
        private:
            Q_PROPERTY(int version READ getVersion FINAL)
        private:
            
        public:
            StaticGlobal();
            ~StaticGlobal();
        public:
            int getVersion() const;
        private:
            sstd_class(StaticGlobal);
        };

        class Static : public QObject {
            Q_OBJECT
        private:
            Q_PROPERTY(StaticGlobal* sharedObject READ getSharedObject)
        private:
            Q_PROPERTY(QObject* appStyle READ getAppStyle)
        public:
            inline StaticGlobal *getSharedObject() const;
            inline QObject * getAppStyle() const;
        public:
            Static(StaticGlobal *,QObject *);
            ~Static();
        private:
            StaticGlobal * const thisSharedObject;
            QObject * const thisAppStyle;
        public:
            sstd_class(Static);
        };

        inline StaticGlobal *Static::getSharedObject() const {
            return thisSharedObject;
        }

        inline QObject * Static::getAppStyle() const {
            return thisAppStyle;
        }

    }/*namespace global*/
}/*namespace sstd*/


















