#ifndef ENUM_GAMESTATUS_H
#define ENUM_GAMESTATUS_H

#include <QObject>

namespace GameStatus
{
    Q_NAMESPACE         // required for meta object creation
    enum class e {
        InProgress,
        Checkmate,
        Stalemate,
        Draw
    };
    Q_ENUM_NS(e)  // register the enum in meta object data
}

#endif // ENUM_GAMESTATUS_H
