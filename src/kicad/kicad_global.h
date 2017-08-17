#ifndef KICAD_GLOBAL_H
#define KICAD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(KICAD_LIBRARY)
#  define KICADSHARED_EXPORT Q_DECL_EXPORT
#else
#  define KICADSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // KICAD_GLOBAL_H
