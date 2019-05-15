#pragma once

#include <sstd_library.hpp>
#include <QtCore/qobject.h>
#include <QtQml/qqmlcontext.h>
#include <QtGui/qcolor.h>

namespace sstd {
    namespace global {

        class StaticGlobal : public QObject {
            Q_OBJECT
        private:
            Q_PROPERTY(int version READ getVersion FINAL)
        private:
            Q_PROPERTY(QColor accent READ getAccent NOTIFY accentChanged)
        private:
            Q_PROPERTY(QColor background READ getBackground NOTIFY backgroundChanged)
        private:
            Q_PROPERTY(QColor foreground READ getForeground NOTIFY foregroundChanged)
        private:
            Q_PROPERTY(QColor primary READ getPrimary NOTIFY primaryChanged)
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
        public:
            int    getElevation() const;
            QColor getAccent() const;
            QColor getBackground() const;
            QColor getForeground() const;
            QColor getPrimary() const;
            bool isDark() const;
        public:
            Q_SIGNAL void accentChanged();
            Q_SIGNAL void backgroundChanged();
            Q_SIGNAL void foregroundChanged();
            Q_SIGNAL void primaryChanged();
        private:
            bool thisIsDark{ false };
        private:
            sstd_class(StaticGlobal);
        };

    }/*namespace global*/
}/*namespace sstd*/


















