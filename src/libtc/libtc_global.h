#pragma once

#include <QtCore/qglobal.h>

#if defined(LIBTC_LIBRARY)
#  define LIBTCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBTCSHARED_EXPORT Q_DECL_IMPORT
#endif
