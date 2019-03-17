#include "sstd_qt_qml_quick_library_thread.hpp"
#include <mutex>
#include <shared_mutex>

namespace _theSSTDLibraryMemoryFile {

    using Mutex = std::shared_mutex;
    inline uint threadObjectID();
    inline Mutex & getThreadObjectMutex(QThread *);

}/*namespace _theSSTDLibraryMemoryFile*/

namespace sstd {

    ThreadObject::ThreadObject(){
    }

    ThreadObject::~ThreadObject(){
    }

    SSTD_QT_SYMBOL_DECL ThreadObject * getThreadObject(QThread * arg){

        if(arg==nullptr){
            return nullptr;
        }

        if( arg->isFinished() ){
            return nullptr;
        }

        union {
            QObjectUserData * varAns;
            ThreadObject * varAnsObject;
        };

        {
            std::shared_lock varReadLock{
                _theSSTDLibraryMemoryFile::getThreadObjectMutex(arg) };
            varAns = arg->userData( _theSSTDLibraryMemoryFile::threadObjectID()  );
            if(varAns){
                return static_cast<ThreadObject *>(varAns);
            }
        }

        {
            std::unique_lock varWriteLock{
                _theSSTDLibraryMemoryFile::getThreadObjectMutex(arg) };
            varAns = arg->userData( _theSSTDLibraryMemoryFile::threadObjectID()  );
            if(varAns){
                return static_cast<ThreadObject *>(varAns);
            }
            varAnsObject = sstd_new<ThreadObject>();
            varAnsObject->moveToThread(arg);
            arg->setUserData( _theSSTDLibraryMemoryFile::threadObjectID() , varAnsObject );
        }

        return varAnsObject;

    }

}/*namespace sstd*/

namespace _theSSTDLibraryMemoryFile {

    inline uint threadObjectID(){
        static const uint varAns =
                QObject::registerUserData();
        return varAns;
    }

    template<typename mutex,std::size_t N,std::size_t M >
    inline mutex & getThreadObjectMutex(QThread * arg){
        using mutex_array_type = std::array< mutex , 1<<N >;
        static mutex_array_type * varMutex =/*never delete*/
                sstd_new<mutex_array_type>();
        auto var =
            reinterpret_cast<std::uintptr_t>(arg);
        var >>= M;
        var &= ((1<<N)-1);
        return (*varMutex)[var];
    }

    inline Mutex & getThreadObjectMutex(QThread *arg){
        return getThreadObjectMutex<Mutex,7,2>(arg);
    }

}/*namespace _theSSTDLibraryMemoryFile*/

