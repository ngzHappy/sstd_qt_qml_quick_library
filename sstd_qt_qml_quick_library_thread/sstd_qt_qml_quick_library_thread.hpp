﻿#pragma once

namespace _theSSTDQtQmlQuickLibraryThreadFile {
}/*_theSSTDQtQmlQuickLibraryThreadFile*/

#include "global_sstd_qt_qml_quick_library.hpp"

#include <QtCore/qobject.h>
#include <QtCore/qthread.h>

namespace sstd {

    class SSTD_QT_SYMBOL_DECL QSSTDThread : public QThread {
        Q_OBJECT
    public:
        QSSTDThread();
    private:
        sstd_class(QSSTDThread);
    };

    class SSTD_QT_SYMBOL_DECL ThreadObject :
        public QObject,
        public QObjectUserData {
        Q_OBJECT
    public:
        ThreadObject();
        ~ThreadObject();
    public:
        using Mutex = std::shared_mutex;
        static Mutex & getThreadObjectMutex(QThread*);
    private:
        sstd_class(ThreadObject);
    };

    SSTD_QT_SYMBOL_DECL ThreadObject * getThreadObject(QThread *);

    class YieldToObjectThreadPrivate;
    class SSTD_QT_SYMBOL_DECL YieldToObjectThread :
        public YieldFunctionBasic,
        public std::enable_shared_from_this< YieldToObjectThread > {
        sstd_delete_copy_create(YieldToObjectThread);
        using shared_super = std::enable_shared_from_this< YieldToObjectThread >;
    private:
        YieldToObjectThreadPrivate * const thisPrivate;
    public:
        virtual ~YieldToObjectThread();
        YieldToObjectThread(std::size_t = 1024uLL * 1024uLL * 64uLL);
    public:
        void start();
    protected:
        /*如果target所在线程就是当前线程则继续执行，
        否则切换到target线程执行*/
        void yieldToObjectThread(QObject*target);
    private:
        void directRun();
        void directYield();
        void directResume();
    private:
        using shared_super::shared_from_this;
        using shared_super::weak_from_this;
    private:
        sstd_class(YieldToObjectThread);
    };

    SSTD_QT_SYMBOL_DECL void lock(QThread *);
    SSTD_QT_SYMBOL_DECL void unlock(QThread *);
    SSTD_QT_SYMBOL_DECL void unlock_later(QThread *);
    SSTD_QT_SYMBOL_DECL void qobjectOwnQThread(QObject*, QThread *, bool = true);

    class QThreadLocker {
        QThread * thisData{ nullptr };
    public:
        inline QThreadLocker() = default;
        inline QThreadLocker(QThread *);
        inline ~QThreadLocker();
        inline QThreadLocker(QThreadLocker &&);
        inline QThreadLocker&operator=(QThreadLocker&&);
    private:
        inline void lock();
        inline void unlock();
    public:
        QThreadLocker(const QThreadLocker &) = delete;
        QThreadLocker&operator=(const QThreadLocker &) = delete;
    public:
        sstd_class(QThreadLocker);
    };

    inline QThreadLocker::QThreadLocker(QThread * arg) : thisData{ arg } {
        this->lock();
    }

    inline QThreadLocker::~QThreadLocker() {
        this->unlock();
    }

    inline QThreadLocker::QThreadLocker(QThreadLocker && arg) : thisData(arg.thisData) {
        arg.thisData = nullptr;
    }

    inline QThreadLocker&QThreadLocker::operator=(QThreadLocker&& arg) {
        if (this == &arg) {
            return *this;
        }
        this->unlock();
        this->thisData = arg.thisData;
        arg.thisData = nullptr;
        return *this;
    }

    inline void QThreadLocker::lock() {
        if (thisData) {
            sstd::lock(thisData);
        }
    }

    inline void QThreadLocker::unlock() {
        if (thisData) {
            sstd::unlock(thisData);
        }
    }

}/*namespace sstd*/
