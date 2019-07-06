#include "sstd_qt_qml_quick_library_dynamic_property.hpp"

namespace sstd{

#ifndef Q_MOC_RUN
    using Mutex = \uacae_dynamic_property_detail::Mutex;
#endif

    inline uint theObjectID() {
        static const uint varAns = []() ->uint{
                return QObject::registerUserData();
    }();
        return varAns;
    }

    /*必须是2的整数倍*/
    inline constexpr int theMutexSize(){
        return 256;
    }

    inline Mutex * theMutex(){
        using T = std::array< Mutex, theMutexSize()>;
        auto static globalAns = sstd_new<T>();
        return globalAns->data();
    }

    inline Mutex * theMutex(const void * arg){
        auto varPointer = reinterpret_cast< std::uintptr_t >(arg);
        return theMutex()+((varPointer>>1)&static_cast<std::uintptr_t>(theMutexSize()-1));
    }

#ifndef Q_MOC_RUN
    namespace \uacae_dynamic_property_detail{

        SSTD_QT_SYMBOL_DECL Mutex * mutex(){
            return theMutex();
        }

        SSTD_QT_SYMBOL_DECL int mutex_size(){
            return theMutexSize();
        }

        SSTD_QT_SYMBOL_DECL uint object_id(){
            return theObjectID();
        }

    }/*\uacae_dynamic_property_detail*/
#endif


    SSTD_QT_SYMBOL_DECL DynamicPropertyMap * getDynamicPropertyMap(QObject * arg){

        if( arg == nullptr ){
            return {};
        }

        auto varMutex = theMutex(arg);
        const auto varID = theObjectID() ;

        {
            std::shared_lock varReadLock{*varMutex};
            auto varUserData = static_cast<DynamicPropertyMap *>( arg->userData( varID ) );
            if(varUserData){
                return varUserData ;
            }
        }

        std::unique_lock varWriteLock{*varMutex};
        {
            auto varUserData = static_cast<DynamicPropertyMap *>( arg->userData( varID ) );
            if(varUserData){
                return varUserData;
            }
        }

        auto varProperty = sstd_new<DynamicPropertyMap>();
        arg->setUserData( varID, varProperty );
        return varProperty;

    }

    bool DynamicPropertyMap::has(const DynamicPropertyMapKey & arg) const{
        std::shared_lock varRead{ thisMutex };
        auto varPos = thisMap.find(arg);
        return varPos!=thisMap.end();
    }

    std::shared_ptr<QObjectUserData> DynamicPropertyMap::get(const DynamicPropertyMapKey & arg) const{
        std::shared_lock varRead{ thisMutex };
        auto varPos = thisMap.find(arg);
        if(varPos == thisMap.end()){
            return {};
        }
        return varPos->second;
    }

    void DynamicPropertyMap::erase(const DynamicPropertyMapKey & arg){
        std::unique_lock varWrite{ thisMutex };
        thisMap.erase(arg);
    }

    void DynamicPropertyMap::put(const DynamicPropertyMapKey & arg,std::shared_ptr<QObjectUserData> argV){
        std::unique_lock varWrite{ thisMutex };
        thisMap[arg]=std::move(argV);
    }

}/*namespce sstd*/

