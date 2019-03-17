#include "sstd_qt_qml_quick_library_thread.hpp"

namespace sstd {

    ThreadObject::ThreadObject(){
    }

    ThreadObject::~ThreadObject(){
    }

    SSTD_QT_SYMBOL_DECL ThreadObject * getThreadObject(QThread *){
        return nullptr;
    }

}/*namespace sstd*/












