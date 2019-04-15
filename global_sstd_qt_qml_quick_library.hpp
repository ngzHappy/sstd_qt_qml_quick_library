#pragma once

#include <sstd_library.hpp>

#if defined(SSTD_QT_BUILD_SOURCE)
#define SSTD_QT_SYMBOL_DECL SSTD_SYMBOL_EXPORT
#else
#define SSTD_QT_SYMBOL_DECL SSTD_SYMBOL_IMPORT
#endif


namespace sstd {

    class DeleteLater{
    public:
        template<typename T>
        inline void operator()(T * arg) const noexcept {
            if (!arg->parent()) {
                arg->deleteLater();
            }
        }
    };

}/*namespace sstd*/

template<typename T,typename ... Args >
inline std::unique_ptr< T , sstd::DeleteLater > sstd_make_deletelater_virtual_unique(Args && ... args){
    return std::unique_ptr< T , sstd::DeleteLater >{
        sstd_virtual_new<T>(std::forward<Args>(args)...) };
}
