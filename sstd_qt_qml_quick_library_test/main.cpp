
#include <sstd_qt_qml_quick_library.hpp>

class ThreadObject :
        public sstd::YieldToObjectThread {
    class ThisData{
    public:
        QThread * thread;
    }thisData;
public:

    inline ~ThreadObject(){
        thisData.thread->quit();
    }

    inline ThreadObject(){
        thisData.thread = sstd_new<QThread>();
        QObject::connect(thisData.thread,&QThread::finished,
                         thisData.thread,&QObject::deleteLater);
        thisData.thread->start();
    }

protected:

    inline void printThreadInfo(){
        std::cout << u8R"(entered!)"sv
                  << std::hex
                  << QThread::currentThread()
                  << std::endl;
    }

    inline void doRun() override {

        auto varCallerThread = QThread::currentThread();
        auto varWorkerThread = thisData.thread;

        printThreadInfo();

        yieldToObjectThread( sstd::getThreadObject( varWorkerThread ) );
        printThreadInfo();

        yieldToObjectThread( sstd::getThreadObject( varCallerThread ) );
        printThreadInfo();

    }

private:
    sstd_class(ThreadObject);
};

inline static void test_run_in_thread(){

    auto varTestThread =
        sstd_make_shared<ThreadObject>();
    varTestThread->start();

}

int main(int argc,char ** argv){

    QApplication varApp{ argc , argv };

    test_run_in_thread();

    QWidget widget;
    widget.show();

    return varApp.exec();

}



















