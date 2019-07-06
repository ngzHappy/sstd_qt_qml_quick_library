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
    }/*\uacae_dynamic_property_detail*/
#endif

    enum class KnowDynamicPropertyMapKey{
        QThreadDataKey,
    };

    using DynamicPropertyMapKeyDetail =
    std::variant< std::uintptr_t, QString ,KnowDynamicPropertyMapKey>;

    class DynamicPropertyMapKey :
            public DynamicPropertyMapKeyDetail {
        using Super = DynamicPropertyMapKeyDetail;
    public:
        inline DynamicPropertyMapKey(){
        }
        inline DynamicPropertyMapKey(std::uintptr_t arg) : Super(arg){
        }
        inline DynamicPropertyMapKey(QString arg) : Super(std::move(arg)){
        }
    private:
        sstd_class(DynamicPropertyMapKey);
    };

    class DynamicPropertyMap;
    SSTD_QT_SYMBOL_DECL std::shared_ptr<DynamicPropertyMap> getDynamicPropertyMap(QObject *);

    class SSTD_QT_SYMBOL_DECL DynamicPropertyMap final :
            public QObjectUserData {
    public:
        std::shared_ptr<QObjectUserData> get(const DynamicPropertyMapKey &) const;
        void put(const DynamicPropertyMapKey &,std::shared_ptr<QObjectUserData>);
    private:
        std::shared_ptr<DynamicPropertyMap> thisData;
        friend std::shared_ptr<DynamicPropertyMap> getDynamicPropertyMap(QObject *);
        mutable std::shared_mutex thisMutex;
        using Map = std::map< DynamicPropertyMapKey,
        std::shared_ptr<QObjectUserData>,
        std::less<>,
        sstd::allocator< std::pair<const DynamicPropertyMapKey,std::shared_ptr<QObjectUserData> > > >;
        Map thisMap;
    private:
        sstd_class(DynamicPropertyMap);
    };

}/*namespace sstd*/


