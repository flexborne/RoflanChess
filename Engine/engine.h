#ifndef ENGINE_H
#define ENGINE_H

#include "defines.h"


// std
#include <vector>
#include <optional>
#include <bitset>
#include <set>
#include <variant>
#include <memory>

// game
#include "gameBoardData.h"
#include "enum_GameStatus.h"
#include "defines.h"
#include "smallPieceInfo.h"
#include "error.h"



class Piece;
class Move;

class Engine
{
    struct BoardStateCache;

public:
    /// @brief action to perform to the piece
    enum class PieceAction {
        Create,
        Remove,
        Move
    };

    struct Result
    {
        std::set<Move> allPossibleMoves;
        std::set<std::pair<SmallPieceInfoEx, PieceAction>> pieceActionsToPerofrm;

        Side::e sideToMove;
        std::bitset<4> castleRights;
        std::optional<uint8_t> enPassantSquare;

        std::optional<Side::e> checkmatedSide = std::nullopt;
        GameStatus::e gameStatus = GameStatus::e::InProgress;
    };

    using EngineResult = std::variant<Error, Result>;

    Engine() noexcept;

    ~Engine();

    auto performMove(GameBoardData&& gameBoardData, Move&& move) -> EngineResult;

    /// @return is current given square attacked by given side
    bool isSquareAttacked(const std::unique_ptr<BoardStateCache>& boardState, uint8_t square, Side::e side) const;

    std::optional<uint8_t> getEnPassantSquare(const std::unique_ptr<BoardStateCache>& boardState) const;

    const std::bitset<4>& getCastleRights(const std::unique_ptr<BoardStateCache>& boardState) const;

    auto generateAllAvailableMoves(GameBoardData&& gameBoardData) noexcept -> std::set<Move>;
    auto generateAllAvailableMoves(const std::unique_ptr<BoardStateCache>& boardState) noexcept -> std::set<Move>;


private:
    void updateBoardState(GameBoardData&& gameBoardData);
    void updateOccupancies(std::unique_ptr<BoardStateCache>& cache);
    void clearCache();

    uint64_t getOccupancy(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const;
    uint64_t& getBitboard(const std::unique_ptr<BoardStateCache>& boardState, PieceType type, Side::e side) const;

    /// @return set of all possible PseudoLegalMoves
    auto generateAllPseudoLegalMoves(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const noexcept -> std::set<Move>;

    template <PieceType>
    auto generatePseudoLegalMoves(const std::unique_ptr<BoardStateCache>& boardState, [[maybe_unused]] Side::e side) const -> std::set<Move>;

    auto makeMove(const std::unique_ptr<BoardStateCache>& boardState, const Move& move) -> bool;

private:
    std::unique_ptr<BoardStateCache> currentBoardStateCache;
    std::map<Move, std::unique_ptr<BoardStateCache>> moveResults;
    std::map<Move, std::set<std::pair<SmallPieceInfoEx, PieceAction>>> pieceActionResults;

};
#endif // ENGINE_H
