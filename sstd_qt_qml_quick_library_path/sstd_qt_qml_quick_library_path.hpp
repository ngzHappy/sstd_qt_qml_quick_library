#pragma once

#include "../global_sstd_qt_qml_quick_library.hpp"
#include <QtCore/qurl.h>
#include <QtCore/qdir.h>
#include <QtCore/qcoreapplication.h>

namespace sstd {

#if defined(_DEBUG) && defined(CURRENT_DEBUG_PATH)
    SSTD_QT_SYMBOL_DECL QUrl getLocalFileFullPath(const QString &, const QString & = CURRENT_DEBUG_PATH);
#else
    SSTD_QT_SYMBOL_DECL QUrl getLocalFileFullPath(const QString &, const QString & = qApp->applicationDirPath());
#endif

#if defined(_DEBUG) && defined(CURRENT_DEBUG_PATH)
    SSTD_QT_SYMBOL_DECL QString getLocalFileFullFilePath(const QString &, const QString & = CURRENT_DEBUG_PATH);
#else
    SSTD_QT_SYMBOL_DECL QString getLocalFileFullFilePath(const QString &, const QString & = qApp->applicationDirPath());
#endif

#if defined(_DEBUG) && defined(CURRENT_DEBUG_PATH)
    SSTD_QT_SYMBOL_DECL sstd::detail::utf8String getLocalFileFullFilePath(std::string_view, std::string_view = CURRENT_DEBUG_PATH);
#else
    SSTD_QT_SYMBOL_DECL sstd::detail::utf8String getLocalFileFullFilePath(std::string_view, std::string_view = {});
#endif

    SSTD_QT_SYMBOL_DECL QString getLocalPathFromApp(const QString &);

    template<typename T>
    inline std::remove_reference_t<T> autoLocalPath(const QString & arg) {
        using U = std::remove_cv_t< std::remove_reference_t<T> >;
        if constexpr (std::is_same_v<U, QString>) {
            return getLocalFileFullFilePath(arg);
        } else if constexpr (std::is_same_v<U, QUrl>) {
            return getLocalFileFullPath(arg);
        }
    }

    template<typename T>
    inline std::remove_reference_t<T> autoTheQMlPath(const QString & arg) {
        using U = std::remove_cv_t< std::remove_reference_t<T> >;
        const static QString varDir = []()->QString {
            QDir varDir{ qApp->applicationDirPath() };
#if defined(_DEBUG)
            return varDir.absoluteFilePath(QStringLiteral("theqml_the_debug"));
#else
            return varDir.absoluteFilePath(QStringLiteral("theqml"));
#endif
        }();
        if constexpr (std::is_same_v<U, QString>) {
            return getLocalFileFullFilePath(arg, varDir);
        } else if constexpr (std::is_same_v<U, QUrl>) {
            return getLocalFileFullPath(arg, varDir);
        }
    }

}/*namespace sstd*/
