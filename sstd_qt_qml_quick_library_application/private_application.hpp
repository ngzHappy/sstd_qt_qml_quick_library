#pragma once

#include <sstd_library.hpp>
#include <QtCore/qobject.h>
#include <QtQml/qqmlcontext.h>
#include <QtGui/qcolor.h>
#include <QtQuick/qquickwindow.h>
#include <QtCore/qpointer.h>
#include <QtQml/qqmlapplicationengine.h>
#include <chrono>
#include "sstd_qquickmaterialstyle_p.h"
class QCoreApplication;

namespace sstd {
    namespace global {

        class StaticGlobal : public sstd::QQuickMaterialStyle {
            Q_OBJECT
        private:
            Q_PROPERTY(int version READ getVersion CONSTANT FINAL)
        private:
            Q_PROPERTY(bool isDark READ isDark WRITE setIsDark NOTIFY isDarkChanged)
        private:
            Q_PROPERTY(QQuickWindow* privateDefaultWindow READ getPrivateDefaultWindow WRITE setPrivateDefaultWindow NOTIFY privateDefaultWindowChanged)
        public:
            StaticGlobal();
            ~StaticGlobal();
        public:
            int getVersion() const;
        public:
            void setIsDark(bool);
            void setPrivateDefaultWindow(QQuickWindow *);
            Q_SIGNAL void isDarkChanged();
            Q_SIGNAL void privateDefaultWindowChanged();
            inline bool isDark() const;
            inline QQuickWindow * getPrivateDefaultWindow() const;
        public:
            Q_INVOKABLE QString readLocalFile(const QUrl & argLocalUrl);
        public:
            Q_SLOT QVariant timeSinceCreate() const;
            Q_SLOT void reloadDefaultStyle();
        private:
            bool thisIsDark{ true };
            std::chrono::steady_clock::time_point const thisStart;
            QPointer< QQuickWindow > thisPrivateDefaultWindow;
        private:
            void privateUpdateTheme();
        private:
            sstd_class(StaticGlobal);
        };

        inline bool StaticGlobal::isDark() const {
            return thisIsDark;
        }

        inline QQuickWindow *StaticGlobal::getPrivateDefaultWindow() const {
            return thisPrivateDefaultWindow.data();
        }

    }/*namespace global*/

    class StyledQQmlApplicationEngine : public QQmlApplicationEngine {
        Q_OBJECT
    private:
        using Super = QQmlApplicationEngine;
    public:
        StyledQQmlApplicationEngine(QCoreApplication *);
    private:
        sstd_class(StyledQQmlApplicationEngine);
    };

}/*namespace sstd*/
