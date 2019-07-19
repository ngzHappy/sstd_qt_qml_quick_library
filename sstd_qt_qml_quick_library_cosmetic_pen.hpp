#pragma once

#include <sstd_library.hpp>
#include <QtGui/qpen.h>

namespace sstd {

    namespace the {
        class SetCosmeticPen {
        public:
            inline void operator()(QPen * arg) const {
                arg->setCosmetic(true);
            }
        };
    }/*namespace the*/

    using CosmeticPen = ElementAt<QPen, the::SetCosmeticPen>;

}/*namespace sstd*/







