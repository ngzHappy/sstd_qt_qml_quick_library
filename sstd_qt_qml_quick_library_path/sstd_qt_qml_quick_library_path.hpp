#pragma once

#include "../global_sstd_qt_qml_quick_library.hpp"
#include <QtCore/qurl.h>
#include <QtCore/qdir.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qregularexpression.h>

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

    /*qApp未初始化时使用*/
    inline QString replaceFileName(const char * argV, const QString & arg) {
        auto varWString = [arg]()->std::wstring {
            const auto varLength = static_cast<std::size_t>(arg.size());
            std::wstring varWString;
            varWString.resize(1 + varLength);
            arg.toWCharArray(varWString.data());
            varWString.resize(varLength);
            assert((varWString.size() - static_cast<std::size_t>(arg.size())) < 1024u);
            return std::move(varWString);
        };
#if defined(_DEBUG) && defined(CURRENT_DEBUG_PATH)
        sstd::filesystem::path varPath{ CURRENT_DEBUG_PATH };
        varPath /= varWString();
        (void)argV;
#else
        sstd::filesystem::path varPath{ argV };
        varPath = varPath.parent_path() / varWString();
#endif
        const auto varWStringAns = varPath.wstring();
        return QString::fromStdWString(varWStringAns);
    }

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
    QString varStr = arg;
    const static auto varReg = QRegularExpression(QStringLiteral("_the_debug"),
        QRegularExpression::CaseInsensitiveOption);
    varStr.replace(varReg, QStringLiteral(""));
    if constexpr (std::is_same_v<U, QString>) {
        return getLocalFileFullFilePath(varStr, varDir);
    } else if constexpr (std::is_same_v<U, QUrl>) {
        return getLocalFileFullPath(varStr, varDir);
    }
}

}/*namespace sstd*/
