#pragma once

#include "../sstd_qt_qml_quick_library.hpp"

namespace sstd {

    class SSTD_QT_SYMBOL_DECL BeforeAfterQtApplication {
    public:
        BeforeAfterQtApplication();
        ~BeforeAfterQtApplication();
    public:
        sstd_delete_copy_create(BeforeAfterQtApplication);
    private:
        sstd_class(BeforeAfterQtApplication);
    };

    /*不同于QApplication能够被多次构造和析构，
    QtApplication只能被构造和析构一次*/
    class SSTD_QT_SYMBOL_DECL QtApplication :
        public QApplication,
        public sstd::Application {
        Q_OBJECT
    public:
        QtApplication(int &, char **, BeforeAfterQtApplication&& = {});
    private:
        sstd_class(QtApplication);
    };

}/*namespace sstd*/
