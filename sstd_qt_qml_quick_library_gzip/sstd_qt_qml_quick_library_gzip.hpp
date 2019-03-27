#pragma once

#include "../global_sstd_qt_qml_quick_library.hpp"

#include <QByteArray>
#include <memory>

namespace sstd {

    class SSTD_QT_SYMBOL_DECL GZipCompressor {
    public:

        static bool gzipCompress(const char *, int, QByteArray &, int level = -1);
        static bool gzipDecompress(const char *, int, QByteArray &);

        static inline bool gzipCompress(const QByteArray & a, QByteArray & b, int c = -1) {
            return gzipCompress(a.constData(), a.length(), b, c);
        }

        static inline bool gzipDecompress(const QByteArray & a, QByteArray & b) {
            return gzipDecompress(a.constData(), a.length(), b);
        }

        template<typename T>
        static inline QByteArray gzipDecompress(T && input) {
            QByteArray ans;
            if (gzipDecompress(std::forward<T>(input), ans)) {
                return std::move(ans);
            }
            ans.clear();
            return ans;
        }

    private:
        sstd_class(GZipCompressor);
    };

}/*namespace sstd*/


