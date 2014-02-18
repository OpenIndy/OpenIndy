#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(OI_LIB)
#  define OI_LIB_EXPORT Q_DECL_EXPORT
#else
#  define OI_LIB_EXPORT Q_DECL_IMPORT
#endif

#endif // GLOBAL_H
