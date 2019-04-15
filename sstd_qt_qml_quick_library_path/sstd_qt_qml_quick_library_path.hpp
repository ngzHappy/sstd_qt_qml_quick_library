#pragma once

#include "../global_sstd_qt_qml_quick_library.hpp"
#include <QtCore/qurl.h>
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


}/*namespace sstd*/
