#ifndef PIECE_H
#define PIECE_H

#include "defines.h"

#include <set>
#include <map>
#include <utility>

//strings
#include <string>
#include <string_view>
#include <QString>
#include "move.h"
#include "defines.h"
class Piece
{
    using enum PieceType;
public:
    explicit Piece(Side::e  side, uint8_t position) noexcept;

    virtual ~Piece();

    auto getAvailableMoves() const -> const std::set<Move>&;
    auto getAvailableMoves() -> std::set<Move>&;

    void clearMoves();

    void addAvailableMove(Move move);

    auto getPosition() const -> uint8_t;


    int vertPosisiton() const;

    void setPosition(int pos) {
        position = pos;
    }

    QString getImagePath() const noexcept;
    void setImagePath(std::string_view imagePath);

    virtual PieceType getType() const noexcept = 0;

    Side::e  getSide() const noexcept;

protected:
    void setImagePath(PieceType type) noexcept;

protected:
    std::string imagePath;
    Side::e  side = Side::e::White;

    uint8_t value;

    std::set<Move> availableMoves;
    uint8_t position;
};

#endif // PIECE_H
