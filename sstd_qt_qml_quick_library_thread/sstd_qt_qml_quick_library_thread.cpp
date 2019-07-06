#include "sstd_qt_qml_quick_library_thread.hpp"
#include "../sstd_qt_qml_quick_library_dynamic_property/sstd_qt_qml_quick_library_dynamic_property.hpp"

#include <private/qobject_p.h>
#include <private/qthread_p.h>

#include <set>
#include <mutex>
#include <shared_mutex>

#include <sstd/boost/context/fiber.hpp>
#include <sstd/boost/context/protected_fixedsize_stack.hpp>

namespace sstd {
    using SkipException = boost::context::detail::forced_unwind;
}/*namespace sstd */

namespace _theSSTDLibraryMemoryFile {

    using Mutex = sstd::ThreadObject::Mutex;

}/*namespace _theSSTDLibraryMemoryFile*/

namespace sstd {

    class YieldToObjectThreadPrivate {
    public:
        using fiber_t = boost::context::fiber;
        std::mutex fiberMutex;
        std::optional< fiber_t > fiber;
        fiber_t * functionFiber{ nullptr };
    private:
        sstd_class(YieldToObjectThreadPrivate);
    };

    YieldToObjectThread::YieldToObjectThread(std::size_t argStackSize) :
        thisPrivate(sstd_new<YieldToObjectThreadPrivate>()) {
        using fiber_t = YieldToObjectThreadPrivate::fiber_t;
        thisPrivate->fiber.emplace(
            std::allocator_arg,
            boost::context::protected_fixedsize_stack{ argStackSize },
            [this](fiber_t && f)->fiber_t {
            thisPrivate->functionFiber = &f;
            this->directRun();
            return std::move(f);
        });
    }

    void YieldToObjectThread::start() {
        directResume();
    }

    YieldToObjectThread::~YieldToObjectThread() {
        delete thisPrivate;
    }

    /*在同一时刻只能有一个调用者,也只能有一个执行者*/
    void YieldToObjectThread::directResume() {/*将执行权交给执行者*/
        std::unique_lock varLock{ thisPrivate->fiberMutex };
        assert(thisPrivate->fiber);
        assert(*(thisPrivate->fiber));
        (*(thisPrivate->fiber)) = std::move(*(thisPrivate->fiber)).resume();
    }

    void YieldToObjectThread::directYield() {/*将执行权交给调用者*/
        assert(thisPrivate->functionFiber);
        assert(*(thisPrivate->functionFiber));
        (*(thisPrivate->functionFiber)) = std::move(*(thisPrivate->functionFiber)).resume();
    }

    void YieldToObjectThread::yieldToObjectThread(QObject * arg) {

        assert(arg);
        assert(this->shared_from_this());

        if (QThread::currentThread() == arg->thread()) {
            /*继续执行*/
            return;
        }

        /*切换调用线程*/
        QMetaObject::invokeMethod(arg, [varThis = this->shared_from_this()]() {
            varThis->directResume();
        }, Qt::QueuedConnection);

        directYield();

    }

    void YieldToObjectThread::directRun() {
        sstd_try{
            this->doRun();
        }sstd_catch(const SkipException &) {
            throw;
        } sstd_catch(...) {
            this->doException();
        }
    }

    ThreadObject::ThreadObject() {
    }

    ThreadObject::~ThreadObject() {
    }

    SSTD_QT_SYMBOL_DECL ThreadObject * getThreadObject(QThread * arg) {

        if (arg == nullptr) {
            return nullptr;
        }

        if (arg->isFinished()) {
            return nullptr;
        }

#ifndef Q_MOC_RUN
        using T = sstd::\uacae_dynamic_property_detail::KnowDynamicPropertyMapKey;
#endif
        auto varMap = sstd::getDynamicPropertyMap(arg);
        auto varAns = varMap->construct<ThreadObject>(T::QThreadDataKey);
       if(varAns.second) {
           varAns.first->moveToThread(arg);
       }
       return varAns.first;

    }

    SSTD_QT_SYMBOL_DECL void lock(QThread * arg) {
        assert(qApp);
        if (arg == qApp->thread()) {
            return;
        }
        auto var =
            static_cast<QThreadPrivate *>(QObjectPrivate::get(arg));
        var->ref();
    }

    SSTD_QT_SYMBOL_DECL void unlock(QThread * arg) {
        assert(qApp);
        if (arg == qApp->thread()) {
            return;
        }
        auto var =
            static_cast<QThreadPrivate *>(QObjectPrivate::get(arg));
        var->deref();
    }

    SSTD_QT_SYMBOL_DECL void unlock_later(QThread * arg) {
        assert(qApp);
        if (arg == qApp->thread()) {
            return;
        }
        auto var = getThreadObject(arg);
        QMetaObject::invokeMethod(var, [arg]() { sstd::unlock(arg); }, Qt::QueuedConnection);
    }

    namespace {
        class QObjectOwnQThread : public QObjectUserData {
            std::set< QThread *, std::less<>, sstd::allocator< QThread * > > thisData;
        public:

            inline void insert(QThread * arg) {
                if (thisData.count(arg) > 0) {
                    return;
                }
                thisData.insert(arg);
                sstd::lock(arg);
            }

            inline void remove(QThread * arg) {
                auto varPos = thisData.find(arg);
                if (varPos == thisData.end()) {
                    return;
                }
                thisData.erase(varPos);
                unlock(arg);
            }

            inline ~QObjectOwnQThread() {
                for (auto varI : thisData) {
                    unlock(varI);
                }
            }

            inline QObjectOwnQThread() = default;

        private:
            sstd_class(QObjectOwnQThread);
        };

    }

    SSTD_QT_SYMBOL_DECL void qobjectOwnQThread(QObject * argO, QThread * argT, bool argAdd) {

        if (argO == nullptr) {
            return;
        }

        if (argT == nullptr) {
            return;
        }

        assert(dynamic_cast<QThread *>(argO) == nullptr);

#ifndef Q_MOC_RUN
        using T = sstd::\uacae_dynamic_property_detail::KnowDynamicPropertyMapKey;
#endif

        auto varMap = sstd::getDynamicPropertyMap(argO);
        auto varUserData = varMap->construct<QObjectOwnQThread>(T::QThreadOwnKey);

        if (argAdd) {
            varUserData.first->insert(argT);
        } else {
            varUserData.first->remove(argT);
        }

    }

    QSSTDThread::QSSTDThread() {
        assert(qApp);
        this->moveToThread(qApp->thread());
        {
            auto var = getThreadObject(this);
            (void)var;
        }
        connect(this, &QThread::finished, this, &QObject::deleteLater, Qt::DirectConnection);
    }

}/*namespace sstd*/


