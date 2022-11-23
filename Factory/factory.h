#ifndef FACTORY_H
#define FACTORY_H

// std
#include <memory>
#include <map>
#include <functional>

#include "defines.h"

class Piece;
struct SmallPieceInfo;

class Factory
{
public:
    Factory();

    static std::unique_ptr<Piece> create(SmallPieceInfo);
private:
    static const std::map<PieceType, std::function<std::unique_ptr<Piece>(Side::e, uint8_t)>> mapping;
};

#endif // FACTORY_H
