#include "sstd_qt_qml_quick_library_path.hpp"
#include <QtCore/qdir.h>

namespace sstd {

    SSTD_QT_SYMBOL_DECL QUrl getLocalFileFullPath(
        const QString & argFileName,
        const QString & argBase) {

        const auto varAns = getLocalFileFullFilePath(argFileName, argBase);
        if (varAns.startsWith(QChar('/'))) {
            return QStringLiteral(R"(file://)") + varAns;
        } else {
            return QStringLiteral(R"(file:///)") + varAns;
        }

    }

    SSTD_QT_SYMBOL_DECL QString getLocalFileFullFilePath(
        const QString & argFileName,
        const QString & argBase) {

        const QDir varRootDir{ argBase };
        return varRootDir.absoluteFilePath(argFileName);

    }

    SSTD_QT_SYMBOL_DECL sstd::detail::utf8String getLocalFileFullFilePath(
        std::string_view varName,
        std::string_view varDir) {

        if (varName.empty()) {
            return { };
        }

        if (varDir.empty()) {
            return sstd::detail::utf8String{ varName };
        }

        bool varNameOp;
        bool varDirOp;

        varNameOp = ((varName[0] == '/') || (varName[0] == '\\'));
        {
            auto varChar = *varDir.rbegin();
            varDirOp = ((varChar == '/') || (varChar == '\\'));
        }

        if (varDirOp&&varNameOp) {
            sstd::detail::utf8String varAns;
            varAns.reserve(varName.size() + varDir.size());
            varAns += varDir;
            varAns.resize(varDir.size() - 1);
            varAns += varName;
            return std::move(varAns);
        }

        sstd::detail::utf8String varAns;
        varAns.reserve(varName.size() + varDir.size() + 1);
        varAns += varDir;
        if ((varDirOp == false) && (false == varNameOp)) {
            varAns += '/';
        }
        varAns += varName;
        return std::move(varAns);

    }

    SSTD_QT_SYMBOL_DECL QString getLocalPathFromApp(const QString & arg) {
        const static QDir varDir{ qApp->applicationDirPath() };
        return varDir.absoluteFilePath(arg);
    }

}










