#include "sstd_qt_qml_quick_library_thread.hpp"
#include <mutex>
#include <shared_mutex>

#include <boost/context/fiber.hpp>
#include <boost/context/protected_fixedsize_stack.hpp>

namespace _theSSTDLibraryMemoryFile {

    using Mutex = sstd::ThreadObject::Mutex;
    inline uint threadObjectID();
    inline Mutex & getThreadObjectMutex(QThread *);

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

    void YieldToObjectThread::start() noexcept {
        directResume();
    }

    YieldToObjectThread::~YieldToObjectThread() {
        delete thisPrivate;
    }

    /*在同一时刻只能有一个调用者,也只能有一个执行者*/
    void YieldToObjectThread::directResume() noexcept {/*将执行权交给执行者*/
        std::unique_lock varLock{ thisPrivate->fiberMutex };
        assert(thisPrivate->fiber);
        assert(*(thisPrivate->fiber));
        (*(thisPrivate->fiber)) = std::move(*(thisPrivate->fiber)).resume();
    }

    void YieldToObjectThread::directYield() noexcept {/*将执行权交给调用者*/
        assert(thisPrivate->functionFiber);
        assert(*(thisPrivate->functionFiber));
        (*(thisPrivate->functionFiber)) = std::move(*(thisPrivate->functionFiber)).resume();
    }

    void YieldToObjectThread::yieldToObjectThread(QObject * arg) noexcept {

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

    void YieldToObjectThread::directRun() noexcept {
        sstd_try{
            this->doRun();
        }sstd_catch(...) {
            sstd_on_exception();
        }
    }

    ThreadObject::Mutex & ThreadObject::getThreadObjectMutex(QThread* arg) {
        return _theSSTDLibraryMemoryFile::getThreadObjectMutex(arg);
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

        union {
            QObjectUserData * varAns;
            ThreadObject * varAnsObject;
        };

        auto & varMutex = _theSSTDLibraryMemoryFile::getThreadObjectMutex(arg);
        const auto varID = _theSSTDLibraryMemoryFile::threadObjectID();

        {
            std::shared_lock varReadLock{ varMutex };
            varAns = arg->userData(varID);
        }

        if (varAns) {
            return static_cast<ThreadObject *>(varAns);
        }

        {
            std::unique_lock varWriteLock{ varMutex };
            varAns = arg->userData(varID);
            if (varAns) {
                return static_cast<ThreadObject *>(varAns);
            }
            varAnsObject = sstd_new<ThreadObject>();
            varAnsObject->moveToThread(arg);
            arg->setUserData(varID, varAnsObject);
        }

        return varAnsObject;

    }

}/*namespace sstd*/

namespace _theSSTDLibraryMemoryFile {

    inline uint threadObjectID() {
        static const uint varAns =
            QObject::registerUserData();
        return varAns;
    }

    template<typename mutex, std::size_t N, std::size_t M >
    inline mutex & getThreadObjectMutex(QThread * arg) {
        using mutex_array_type = std::array< mutex, 1 << N >;
        static mutex_array_type * varMutex =/*never delete*/
            sstd_new<mutex_array_type>();
        auto var =
            reinterpret_cast<std::uintptr_t>(arg);
        var >>= M;
        var &= ((1 << N) - 1);
        return (*varMutex)[var];
    }

    inline Mutex & getThreadObjectMutex(QThread *arg) {
        return getThreadObjectMutex<Mutex, 7, 2>(arg);
    }

}/*namespace _theSSTDLibraryMemoryFile*/

