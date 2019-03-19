
#include <sstd_qt_qml_quick_library.hpp>

class ThreadObject final :
    public sstd::YieldToObjectThread {
    class ThisData {
    public:
        QThread * thread;
    }thisData;
public:

    inline ~ThreadObject() {
        thisData.thread->quit();
    }

    inline ThreadObject() {
        thisData.thread = sstd_new<QThread>();
        QObject::connect(thisData.thread, &QThread::finished,
            thisData.thread, &QObject::deleteLater);
        thisData.thread->start();
    }

protected:

    inline void printThreadInfo() {
        std::cout << u8R"(entered!)"sv
            << std::hex
            << QThread::currentThread()
            << std::endl;
    }

    inline void doRun() override {

        auto varCallerThread = QThread::currentThread();
        auto varWorkerThread = thisData.thread;

        printThreadInfo();

        yieldToObjectThread(sstd::getThreadObject(varWorkerThread));
        printThreadInfo();

        yieldToObjectThread(sstd::getThreadObject(varCallerThread));
        printThreadInfo();

    }

private:
    sstd_class(ThreadObject);
};

inline static void test_run_in_thread() {

    {
        /*测试执行*/
        auto varTestThread =
            sstd_make_start_function<ThreadObject>();
        varTestThread();
    }

    {
        /*测试只构造不执行*/
        auto varTestThread =
            sstd_make_start_function<ThreadObject>();
    }

}

class GetBaidu final : public sstd::YieldResumeFunction {
protected:
    inline void doRun() override {

        auto varManager= sstd_make_deletelater_virtual_unique< QNetworkAccessManager >();

        QNetworkRequest varBaidu{ QStringLiteral(R"(http://www.baidu.com)") };

        auto varReply = varManager->get(varBaidu);
        std::optional< QByteArray > varBaiduData;

        QObject::connect(varReply, &QNetworkReply::finished,
           bindFunctionWithThis( [varReply, &varBaiduData, this]() {
            varReply->deleteLater();
            varBaiduData = varReply->readAll();
            sstd_function_resume();
        }));

        sstd_function_yield();

        if (varBaiduData) {
            qDebug() << QStringLiteral(R"(get baidu : )") << (*varBaiduData).size() ;
        } else {
            qDebug() << QStringLiteral(R"(thre is some error when get http://www.baidu.com)");
        }

    }

public:
    inline virtual ~GetBaidu(){
        std::cout  << __func__ << std::endl;
    }


};


class GetBaiduTestException final : public sstd::YieldResumeFunction {
protected:
    inline void doRun() override {

        auto varManager= sstd_make_deletelater_virtual_unique< QNetworkAccessManager >();

        QNetworkRequest varBaidu{ QStringLiteral(R"(http://www.baidu.com)") };

        auto varReply = varManager->get(varBaidu);
        std::optional< QByteArray > varBaiduData;

        QObject::connect(varReply, &QNetworkReply::finished,
           bindFunctionWithThis( [varReply,this]() {
            varReply->deleteLater();
            throw 12345;
             
        }));

        sstd_function_yield();

        //throw 342342;

        if (varBaiduData) {
            qDebug() << QStringLiteral(R"(get baidu : )") << (*varBaiduData).size() ;
        } else {
            qDebug() << QStringLiteral(R"(thre is some error when get http://www.baidu.com)");
        }

    }

public:
    inline virtual ~GetBaiduTestException(){
        std::cout  << __func__ << std::endl;
    }


};

inline static void get_baidu() {

    if constexpr(true){
        auto var =
            sstd_make_start_function<GetBaidu>();
        var();
    }

    if constexpr (true) {
        auto var =
            sstd_make_start_function<GetBaidu>();
    }

    {
        auto var =
                sstd_make_start_function<GetBaiduTestException>();
        auto var1 = var;
        var1();
    }

}


int main(int argc, char ** argv) {

    QApplication varApp{ argc , argv };

    test_run_in_thread();
    get_baidu();

    QWidget widget;
    widget.show();

    return varApp.exec();

}



















