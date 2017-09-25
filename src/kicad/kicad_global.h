#ifndef KICAD_GLOBAL_H
#define KICAD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(KICAD_EXPORT_LIB)
 #define KICAD_EXPORT Q_DECL_EXPORT
#else
 #define KICAD_EXPORT Q_DECL_IMPORT
#endif

#endif // KICAD_GLOBAL_H
