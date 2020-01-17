#ifndef PLAYER_GLOBAL_H
#define PLAYER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLAYER_LIBRARY)
#  define PLAYERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLAYERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLAYER_GLOBAL_H
