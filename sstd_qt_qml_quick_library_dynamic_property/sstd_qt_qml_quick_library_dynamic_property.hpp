#pragma once

#include <sstd_library.hpp>
#include "../sstd_qt_qml_quick_library.hpp"
#include <QtCore/qobject.h>
#include <map>

namespace sstd{

#ifndef Q_MOC_RUN
    namespace \uacae_dynamic_property_detail{
        using Mutex = std::shared_mutex;
        SSTD_QT_SYMBOL_DECL Mutex * mutex();
        SSTD_QT_SYMBOL_DECL int mutex_size();
        SSTD_QT_SYMBOL_DECL uint object_id();
        enum class KnowDynamicPropertyMapKey : std::uint32_t {
            QThreadDataKey,
            QThreadOwnKey,
        };
    }/*\uacae_dynamic_property_detail*/
#endif

#ifndef Q_MOC_RUN
    using DynamicPropertyMapKeyDetail =
    std::variant< std::intptr_t ,
    QString ,
    \uacae_dynamic_property_detail::KnowDynamicPropertyMapKey>;
#endif

    class DynamicPropertyMapKey :
            public DynamicPropertyMapKeyDetail {
        using Super = DynamicPropertyMapKeyDetail;
    public:
        inline DynamicPropertyMapKey(){
        }
        inline DynamicPropertyMapKey(std::intptr_t arg) : Super(arg){
        }
        inline DynamicPropertyMapKey(QString arg) : Super(std::move(arg)){
        }
#ifndef Q_MOC_RUN
        inline DynamicPropertyMapKey(\uacae_dynamic_property_detail::KnowDynamicPropertyMapKey arg):Super(arg){
        }
#endif
    private:
        sstd_class(DynamicPropertyMapKey);
    };

    class DynamicPropertyMap;
    SSTD_QT_SYMBOL_DECL DynamicPropertyMap * getDynamicPropertyMap(QObject *);

    class SSTD_QT_SYMBOL_DECL DynamicPropertyMap final :
            public QObjectUserData {
        using ThisQObjectUserDataPointer = QObjectUserData *;
        using OwnThisQObjectUserDataPointer = std::unique_ptr<QObjectUserData>;
    public:
        bool has(const DynamicPropertyMapKey &) const;
        void erase(const DynamicPropertyMapKey &);
        ThisQObjectUserDataPointer get(const DynamicPropertyMapKey &) const;
        void put(const DynamicPropertyMapKey &,ThisQObjectUserDataPointer);
        template<typename T,typename ... Args >
        inline std::pair<T *,bool> construct(const DynamicPropertyMapKey &,Args && ...);
        DynamicPropertyMap();
        ~DynamicPropertyMap();
    private:
        friend DynamicPropertyMap * getDynamicPropertyMap(QObject *);
        mutable std::shared_mutex thisMutex;
        using Map = std::map< DynamicPropertyMapKey,
        OwnThisQObjectUserDataPointer,
        std::less<>,
        sstd::allocator< std::pair<const DynamicPropertyMapKey,OwnThisQObjectUserDataPointer > > >;
        Map thisMap;
    private:
        sstd_class(DynamicPropertyMap);
    };

    template<typename T,typename ... Args>
    inline std::pair<T *,bool> DynamicPropertyMap::construct(
            const DynamicPropertyMapKey & arg,Args && ...args){
        {
            std::shared_lock varRead{ thisMutex };
            auto varPos = thisMap.find(arg);
            if(varPos!=thisMap.end()){
                return {static_cast<T *>( varPos->second.get() ),false};
            }
        }
        std::unique_lock varWrite{ thisMutex };
        {
            auto varPos = thisMap.find(arg);
            if(varPos!=thisMap.end()){
                return {static_cast<T *>( varPos->second.get() ),false};
            }
        }
        auto varAns = sstd_new<T>(std::forward<Args>(args)...);
        thisMap[arg]=OwnThisQObjectUserDataPointer(varAns);
        return {varAns,true};
    }

}/*namespace sstd*/


