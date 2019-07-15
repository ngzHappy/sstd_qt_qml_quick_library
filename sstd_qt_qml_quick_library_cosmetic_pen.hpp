#pragma once

#include <sstd_library.hpp>
#include <QtGui/qpen.h>

namespace sstd {
    class CosmeticPen : public QPen {
        inline void implConstruct() {
            this->setCosmetic(true);
        }
        template<typename T>
        using rcvr_t = std::remove_cv_t<std::remove_reference_t<T>>;
        template<typename T>
        using is_this = std::is_same<CosmeticPen, rcvr_t<T>>;
        template<typename T>
        inline static constexpr bool is_not_this() {
            return !is_this<T>::value;
        }
    public:
        inline CosmeticPen() {
            this->implConstruct();
        }
        inline CosmeticPen(const CosmeticPen & arg) :
            QPen{ arg } {
            this->implConstruct();
        }
        inline CosmeticPen(CosmeticPen && arg) :
            QPen{ std::move(arg) } {
            this->implConstruct();
        }
        template<typename T0, typename T1, typename ... Args>
        inline CosmeticPen(T0&&arg0, T1&&arg1, Args&&...args) :
            QPen(std::forward<T0>(arg0),
                std::forward<T1>(arg1),
                std::forward<Args>(args)...) {
            this->implConstruct();
        }
        template<typename T0,
            typename = std::enable_if_t<is_not_this<T0>()>>
            inline CosmeticPen(T0&&arg0) :
            QPen(std::forward<T0>(arg0)) {
            this->implConstruct();
        }
        CosmeticPen & operator=(const CosmeticPen &) = default;
        CosmeticPen & operator=(CosmeticPen &&) = default;
    private:
        sstd_class(CosmeticPen);
    };
}/*namespace sstd*/







