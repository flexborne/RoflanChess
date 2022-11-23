#ifndef BOARDDATA_H
#define BOARDDATA_H

#include <QObject>

// std
#include <memory>
#include <vector>

#include "engine.h"
#include "piecesDAO.h"

class Piece;

class BoardManager
{
public:
    explicit BoardManager() noexcept;

    void fetchFenInfo() noexcept;

    auto getPieces() const -> std::vector<const Piece*>;

    auto getPieceAt(uint8_t index) const -> const Piece*;

    auto getSideToMove() const -> Side::e;

    auto getGameStatus() const -> GameStatus::e;

    void performMove(Move&& move);

private:
    void handleEngineResult(Engine::EngineResult&& engineResult);
    void updatePiecesAvailableMoves(const std::set<Move>& availableMoves);

    auto buildGameBoardDTO() const -> GameBoardData;

private:
    std::unique_ptr<Engine> engine;

    /// board data
    PiecesDAO piecesDAO;

    /// 0 - wq, 1 - wk, 2 - bq, 3 - bk
    std::bitset<4> castleRights {0ULL};

    std::optional<uint8_t> enPassantSquare;
    Side::e sideToMove;

    std::optional<Side::e> checkmatedSide = std::nullopt;
    GameStatus::e gameStatus = GameStatus::e::InProgress;
};

#endif // BOARD_H
