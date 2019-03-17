#pragma once

namespace _theSSTDQtQmlQuickLibraryThreadFile {
}/*_theSSTDQtQmlQuickLibraryThreadFile*/

#include "global_sstd_qt_qml_quick_library.hpp"

#include <QtCore/qobject.h>
#include <QtCore/qthread.h>

namespace sstd {

    class SSTD_QT_SYMBOL_DECL ThreadObject :
            public QObject ,
            public QObjectUserData {
        Q_OBJECT
    public:
        ThreadObject();
        ~ThreadObject();
    private:
        sstd_class(ThreadObject);
    };

    SSTD_QT_SYMBOL_DECL ThreadObject * getThreadObject(QThread *);

    class ThreadYieldObjectPrivate;
    class SSTD_QT_SYMBOL_DECL ThreadYieldObject : 
        public std::enable_shared_from_this< ThreadYieldObject > {
        sstd_delete_copy_create(ThreadYieldObject);
    private:
        ThreadYieldObjectPrivate * const thisPrivate;
    public:
        void run() noexcept ;
        virtual ~ThreadYieldObject();
        ThreadYieldObject(std::size_t=1024uLL*1024uLL*64);
    public:
        void yield( QObject * );
    protected:
        virtual void doRun() = 0;
    private:
        sstd_class(ThreadYieldObject);
    };


}/*namespace sstd*/




