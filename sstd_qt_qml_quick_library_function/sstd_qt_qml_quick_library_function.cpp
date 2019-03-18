﻿#include "sstd_qt_qml_quick_library_function.hpp"
#include <boost/context/fiber.hpp>
#include <boost/context/protected_fixedsize_stack.hpp>
namespace sstd {

    class YieldResumeFunctionPrivate{
    public:
        using fiber_t = boost::context::fiber;
        std::optional< fiber_t > fiber;
        fiber_t * fiberFunction{ nullptr };
    private:
        sstd_class(YieldResumeFunctionPrivate);
    };

    YieldResumeFunction::YieldResumeFunction( std::size_t argStackSize ) :
        thisPrivate{ sstd_new<YieldResumeFunctionPrivate>() } {
        using fiber_t = YieldResumeFunctionPrivate::fiber_t;
        thisPrivate->fiber.emplace(std::allocator_arg,
                                   boost::context::protected_fixedsize_stack{argStackSize},
                                   [this](fiber_t &&f)->fiber_t{
            thisPrivate->fiberFunction=&f;
            this->directRun();
            return std::move(f);
        });
    }

    YieldResumeFunction::~YieldResumeFunction(){
        delete thisPrivate;
    }

    void YieldResumeFunction::start() noexcept{
        this->resume();
    }

    void YieldResumeFunction::yield() noexcept{
        this->directYield();
    }

    void YieldResumeFunction::resume() noexcept{
        this->directResume();
    }

    void YieldResumeFunction::directRun() noexcept{
        sstd_try{
            this->doRun();
        }sstd_catch(...){
            sstd_on_exception();
        }
    }

    void YieldResumeFunction::directResume() noexcept{
        assert(thisPrivate->fiber);
        assert(*(thisPrivate->fiber));
        *(thisPrivate->fiber)=std::move(*(thisPrivate->fiber)).resume();
    }

    void YieldResumeFunction::directYield() noexcept{
        assert(thisPrivate->fiberFunction);
        assert(*(thisPrivate->fiberFunction));
        *(thisPrivate->fiberFunction)=std::move(*(thisPrivate->fiberFunction)).resume();
    }

    void YieldResumeFunction::doRun() {
    }

}/*namespace sstd*/






