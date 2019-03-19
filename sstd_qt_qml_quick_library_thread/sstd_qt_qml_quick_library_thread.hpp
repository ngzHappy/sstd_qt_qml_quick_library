#pragma once

namespace _theSSTDQtQmlQuickLibraryThreadFile {
}/*_theSSTDQtQmlQuickLibraryThreadFile*/

#include "global_sstd_qt_qml_quick_library.hpp"

#include <QtCore/qobject.h>
#include <QtCore/qthread.h>

namespace sstd {

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
        void start() noexcept;
    protected:
        /*如果target所在线程就是当前线程则继续执行，
        否则切换到target线程执行*/
        void yieldToObjectThread(QObject*target) noexcept;
    protected:
        void directRun() noexcept override;
    private:
        void directYield() noexcept;
        void directResume() noexcept;
    private:
        using shared_super::shared_from_this;
        using shared_super::weak_from_this;
    private:
        sstd_class(YieldToObjectThread);
    };


}/*namespace sstd*/




