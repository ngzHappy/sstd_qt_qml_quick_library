#pragma once

#include <sstd_library.hpp>
#include <QtCore/qobject.h>
#include <QtQml/qqmlcontext.h>
#include <QtGui/qcolor.h>
#include <chrono>
#include "sstd_qquickmaterialstyle_p.h"

namespace sstd {
    namespace global {

        class StaticGlobal : public sstd::QQuickMaterialStyle {
            Q_OBJECT
        private:
            Q_PROPERTY(int version READ getVersion CONSTANT FINAL)
        private:
            Q_PROPERTY(bool isDark READ isDark WRITE setIsDark NOTIFY isDarkChanged)
        public:
            StaticGlobal();
            ~StaticGlobal();
        public:
            int getVersion() const;
        public:
            void setIsDark(bool);
            Q_SIGNAL void isDarkChanged();
            inline bool isDark() const;
        public:
            Q_SLOT qlonglong timeSinceCreate() const;
        private:
            bool thisIsDark{ false };
            std::chrono::steady_clock::time_point const thisStart;
        private:
            void privateUpdateTheme();
        private:
            sstd_class(StaticGlobal);
        };

        inline bool StaticGlobal::isDark() const{
            return thisIsDark;
        }

    }/*namespace global*/
}/*namespace sstd*/


















