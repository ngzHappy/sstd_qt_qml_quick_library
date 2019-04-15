
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

/*测试正常的yield resume*/
class GetBaidu final : public sstd::YieldResumeFunction {
protected:
    inline void doRun() override {

        auto varManager = sstd_make_deletelater_virtual_unique< QNetworkAccessManager >();

        QNetworkRequest varBaidu{ QStringLiteral(R"(http://www.baidu.com)") };

        auto varReply = varManager->get(varBaidu);
        std::optional< QByteArray > varBaiduData;

        QObject::connect(varReply, &QNetworkReply::finished,
            bind([varReply, &varBaiduData, this]() {
            varReply->deleteLater();
            varBaiduData = varReply->readAll();
        }));
        innerYield();

        if (varBaiduData) {
            qDebug() << QStringLiteral(R"(get baidu : )") << (*varBaiduData).size();
        } else {
            qDebug() << QStringLiteral(R"(thre is some error when get http://www.baidu.com)");
        }

    }

public:
    inline virtual ~GetBaidu() {
        std::cout << __func__ << std::endl;
    }


};

/*测试在inner yield 内部抛出异常*/
class GetBaiduTestException final : public sstd::YieldResumeFunction {
protected:
    inline void doRun() override {

        auto varManager = sstd_make_deletelater_virtual_unique< QNetworkAccessManager >();

        QNetworkRequest varBaidu{ QStringLiteral(R"(http://www.baidu.com)") };

        auto varReply = varManager->get(varBaidu);
        std::optional< QByteArray > varBaiduData;

        QObject::connect(varReply, &QNetworkReply::finished,
            bind([varReply, this]() {
            varReply->deleteLater();
            throw 12345;

        }));

        innerYield();

        //throw 342342;

        assert(false && "this shoule never be called!!!");

    }

public:
    inline virtual ~GetBaiduTestException() {
        std::cout << __func__ << std::endl;
    }


};

/*测试在内部抛出异常*/
class GetBaiduTestException1 final : public sstd::YieldResumeFunction {
protected:
    inline void doRun() override {

        auto varManager = sstd_make_deletelater_virtual_unique< QNetworkAccessManager >();

        QNetworkRequest varBaidu{ QStringLiteral(R"(http://www.baidu.com)") };

        auto varReply = varManager->get(varBaidu);
        std::optional< QByteArray > varBaiduData;

        QObject::connect(varReply, &QNetworkReply::finished,
            bind([varReply, this]() {
            varReply->deleteLater();
        }));

        innerYield();
        throw 342342;

        assert(false && "this shoule never be called!!!");

    }

public:
    inline virtual ~GetBaiduTestException1() {
        std::cout << __func__ << std::endl;
    }


};

/*测试ouuter yield*/
class OuterYieldTest final : public sstd::YieldResumeFunction {
protected:
    inline void doRun() override {

        std::cout << "outer yield" << std::endl;

        sstd_try{
            throw 2222;
        }sstd_catch(...) {
        }

        outerYiled();

        std::cout << "continue" << std::endl;

    }
public:
    inline virtual ~OuterYieldTest() {
        std::cout << __func__ << std::endl;
    }
};

inline static void get_baidu() {

    if constexpr (true) {/*测试构造但不执行*/
        static_assert(_theSSTDLibraryFunctionFile::HasQuit< GetBaidu >::value);
        auto var =
            sstd_make_start_function<GetBaidu>();
    }

    if constexpr (true) {/*测试构造和执行*/
        auto var =
            sstd_make_start_function<GetBaidu>();
        var();
    }

    if constexpr (true) {/*测试内部抛出异常*/
        auto var =
            sstd_make_start_function<GetBaiduTestException>();
        auto var1 = var;
        var1();
    }

    if constexpr (true) {/*测试内部抛出异常*/
        auto var =
            sstd_make_start_function<GetBaiduTestException1>();
        auto var1 = var;
        var1();
    }

    if constexpr (true) {/*outer yield*/
        auto var =
            sstd_make_start_function<OuterYieldTest>();
        auto var1 = var;
        var1.start();
        var1.quit();
    }

    if constexpr (true) {/*outer yield*/
        auto var =
            sstd_make_start_function<OuterYieldTest>();
        auto var1 = var;
        var1.start();
        var1.start();
        var1.start();
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
