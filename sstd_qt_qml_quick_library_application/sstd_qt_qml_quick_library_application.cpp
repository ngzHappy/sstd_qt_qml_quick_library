#include "sstd_qt_qml_quick_library_application.hpp"
#include <exception>

namespace sstd {

    QtApplication::QtApplication(int & argc, char ** argv, BeforeAfterQtApplication&&) :
        QApplication(argc,argv),
        Application(argc,argv){
    }

    /*this function will call before QtApplication construct*/
    BeforeAfterQtApplication::BeforeAfterQtApplication() {

    }

    /*this function will call after QtApplication construct*/
    BeforeAfterQtApplication::~BeforeAfterQtApplication() {
        if (std::uncaught_exceptions()>0) {
            return;
        }
    }

}/*namespace sstd*/
























