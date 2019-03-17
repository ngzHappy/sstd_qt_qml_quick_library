#pragma once

#include <sstd_library.hpp>

#if defined(SSTD_QT_BUILD_SOURCE)
#define SSTD_QT_SYMBOL_DECL SSTD_SYMBOL_EXPORT
#else
#define SSTD_QT_SYMBOL_DECL SSTD_SYMBOL_IMPORT
#endif

