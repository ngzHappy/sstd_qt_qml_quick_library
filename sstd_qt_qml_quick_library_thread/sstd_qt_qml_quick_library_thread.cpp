#include "sstd_qt_qml_quick_library_thread.hpp"
#include <mutex>
#include <shared_mutex>

#include <boost/context/fiber.hpp>
#include <boost/context/protected_fixedsize_stack.hpp>

namespace _theSSTDLibraryMemoryFile {

    using Mutex = std::shared_mutex;
    inline uint threadObjectID();
    inline Mutex & getThreadObjectMutex(QThread *);

}/*namespace _theSSTDLibraryMemoryFile*/

namespace sstd {

    class ThreadYieldObjectPrivate {
    public:
        using fiber_t = boost::context::fiber;
        std::optional< fiber_t > fiber;
        fiber_t * functionFiber{ nullptr };
    private:
        sstd_class(ThreadYieldObjectPrivate);
    };

    ThreadYieldObject::ThreadYieldObject(std::size_t argStackSize) :
        thisPrivate(sstd_new<ThreadYieldObjectPrivate>()) {
        using fiber_t = ThreadYieldObjectPrivate::fiber_t;
        thisPrivate->fiber.emplace(
            std::allocator_arg,
            boost::context::protected_fixedsize_stack{ argStackSize },
            [this](fiber_t && f)->fiber_t {
            thisPrivate->functionFiber = &f;
            this->run();
            return std::move(f);
        });
        (*(thisPrivate->fiber)) = std::move(*(thisPrivate->fiber)).resume();
    }

    ThreadYieldObject::~ThreadYieldObject() {
        delete thisPrivate;
    }

    void ThreadYieldObject::yield(QObject * arg) {

        if (arg == nullptr) {
            return;
        }

        auto varCaller =
            getThreadObject(QThread::currentThread());

        assert(varCaller);
        
        QMetaObject::invokeMethod(varCaller, [varThis = this->shared_from_this(),arg=arg]() {/*加入当前线程队列*/
            QMetaObject::invokeMethod(arg, [varThis]() {/*加入目标线程队列*/
                auto varPrivate = varThis->thisPrivate;
                (*(varPrivate->fiber)) = std::move(*(varPrivate->fiber)).resume();
            }, Qt::QueuedConnection);
        },Qt::QueuedConnection);

        (*(thisPrivate->functionFiber)) = std::move(*(thisPrivate->functionFiber)).resume();

    }

    void ThreadYieldObject::run() noexcept {
        sstd_try{
            this->doRun();
        }sstd_catch(...) {
            sstd_on_exception();
        }
    }

    void ThreadYieldObject::doRun() {
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

        {
            std::shared_lock varReadLock{
                _theSSTDLibraryMemoryFile::getThreadObjectMutex(arg) };
            varAns = arg->userData(_theSSTDLibraryMemoryFile::threadObjectID());
            if (varAns) {
                return static_cast<ThreadObject *>(varAns);
            }
        }

        {
            std::unique_lock varWriteLock{
                _theSSTDLibraryMemoryFile::getThreadObjectMutex(arg) };
            varAns = arg->userData(_theSSTDLibraryMemoryFile::threadObjectID());
            if (varAns) {
                return static_cast<ThreadObject *>(varAns);
            }
            varAnsObject = sstd_new<ThreadObject>();
            varAnsObject->moveToThread(arg);
            arg->setUserData(_theSSTDLibraryMemoryFile::threadObjectID(), varAnsObject);
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

