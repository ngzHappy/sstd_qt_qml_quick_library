#pragma once

#include "../sstd_qt_qml_quick_library.hpp"

namespace sstd {

    class BeforeAfterQtApplicationControl;
    class SSTD_QT_SYMBOL_DECL BeforeAfterQtApplication {
        friend class BeforeAfterQtApplicationControl;
    public:
        BeforeAfterQtApplication(QString = {});
        ~BeforeAfterQtApplication();
    private:
        void construct(const char *);
    public:
        sstd_delete_copy_create(BeforeAfterQtApplication);
    private:
        sstd_class(BeforeAfterQtApplication);
    };

    class QtApplication;
    class SSTD_QT_SYMBOL_DECL BeforeAfterQtApplicationControl {
        friend class QtApplication;
    public:
        BeforeAfterQtApplicationControl(int &, char **, BeforeAfterQtApplication&&);
    private:
        int & thisArgC;
        char ** const thisArgv;
    public:
        sstd_delete_copy_create(BeforeAfterQtApplicationControl);
    private:
        sstd_class(BeforeAfterQtApplicationControl);
    };

    /*不同于QApplication能够被多次构造和析构，
    QtApplication只能被构造和析构一次
    构造顺序：
    1.构造BeforeAfterQtApplication
    2.构造BeforeAfterQtApplicationControl
    3.构造QApplication
    4.构造sstd::Application
    5.析构BeforeAfterQtApplicationControl
    6.析构BeforeAfterQtApplication*/
    class SSTD_QT_SYMBOL_DECL QtApplication :
        public QApplication,
        public sstd::Application {
        Q_OBJECT
    public:
        inline QtApplication(int & argC, char ** argV, BeforeAfterQtApplication&& argAppendArgs = {})
            :QtApplication(BeforeAfterQtApplicationControl{ argC,argV,std::move(argAppendArgs) }) {
        }
    public:
        static QOpenGLContext * getGlobalSharedContex();
    private:
        QtApplication(BeforeAfterQtApplicationControl&&);
    private:
        sstd_class(QtApplication);
    };

    class SSTD_QT_SYMBOL_DECL QtStyledApplication : public QtApplication {
        Q_OBJECT
    public:
        QtStyledApplication(int & argc,char ** argv);
        int exec();
    public:
        QQmlApplicationEngine * getStyledApplicatoinEngine() const;
    private:
        QQmlApplicationEngine * thisAppEngine{nullptr};
    private:
        sstd_class(QtStyledApplication);
    };

}/*namespace sstd*/
