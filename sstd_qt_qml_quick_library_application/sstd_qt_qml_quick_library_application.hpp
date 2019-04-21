#pragma once

#include "../sstd_qt_qml_quick_library.hpp"

namespace sstd {

    class SSTD_QT_SYMBOL_DECL BeforeAfterQtApplication {
    public:
        BeforeAfterQtApplication(QString={});
        ~BeforeAfterQtApplication();
    public:
        void construct(const char *);
    public:
        sstd_delete_copy_create(BeforeAfterQtApplication);
    private:
        sstd_class(BeforeAfterQtApplication);
    };

    /*不同于QApplication能够被多次构造和析构，
    QtApplication只能被构造和析构一次*/
    class SSTD_QT_SYMBOL_DECL _QtApplication :
        public QApplication,
        public sstd::Application {
        Q_OBJECT
    public:
        _QtApplication(int &, char **,BeforeAfterQtApplication&&);
    private:
        sstd_class(_QtApplication);
    };

    class QtApplication {
        std::shared_ptr< _QtApplication > thisData;
    public:
        QtApplication(int &,char **, BeforeAfterQtApplication&& = {});
    public:
        inline int exec();
    public:
        sstd_delete_copy_create(QtApplication);
    private:
        sstd_class(QtApplication);
    };

    inline int QtApplication::exec() {
        return thisData->exec();
    }

}/*namespace sstd*/
