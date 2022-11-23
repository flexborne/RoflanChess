// utils
#include "enumArray.h"

// engine
#include "engine.h"
#include "bitboard.h"
#include "engineDefines.h"

// game
#include "piece.h"
#include "move.h"


// std
#include <ranges>
#include <cstdlib>

// utils
#include "u_defer.h"
#include "utils.h"


namespace {

    /// @return bitboard number according to PieceType and Side
    uint8_t bbNumber(PieceType type, Side::e side) noexcept
    {
        static const std::map<std::pair<PieceType, Side::e>, uint8_t> bitboardNumber {
            { std::pair {PieceType::Pawn, Side::e::White}, 0 },
            { std::pair {PieceType::Knight, Side::e::White}, 1 },
            { std::pair {PieceType::Bishop, Side::e::White}, 2 },
            { std::pair {PieceType::Rook, Side::e::White}, 3 },
            { std::pair {PieceType::Queen, Side::e::White}, 4 },
            { std::pair {PieceType::King, Side::e::White}, 5 },
            { std::pair {PieceType::Pawn, Side::e::Black}, 6 },
            { std::pair {PieceType::Knight, Side::e::Black}, 7 },
            { std::pair {PieceType::Bishop, Side::e::Black}, 8 },
            { std::pair {PieceType::Rook, Side::e::Black}, 9 },
            { std::pair {PieceType::Queen, Side::e::Black}, 10 },
            { std::pair {PieceType::King, Side::e::Black}, 11 }
        };

        return bitboardNumber.find({type, side})->second;
    }

    bool isMoveCapture(MoveType moveType)
    {
        return utils::isAnyOf(moveType, MoveType::Capture,
                              MoveType::CapturePromotion,
                              MoveType::EnPassant);
    }

    bool isMovePromotion(MoveType moveType)
    {
        return utils::isAnyOf(moveType, MoveType::CapturePromotion,
                              MoveType::QuietPromotion);
    }



}   // end of anon namespace


namespace {

    auto oppositeSide(Side::e side) noexcept -> Side::e
    {
        switch (side) {
            case Side::e::Black: return Side::e::White;
            case Side::e::White: return Side::e::Black;
            case Side::e::Both: return Side::e::Both;
        }

        return Side::e::White;
    }

}

struct Engine::BoardStateCache
{
public:
    BoardStateCache() noexcept;

    ~BoardStateCache();

    BoardStateCache(const BoardStateCache& other);

public:
    std::array<uint64_t, N_BITBOARDS> bitboards;    // bitboards for every piece type/side combinations
    std::array<uint64_t, N_OCCUPANCY_BITBOARDS> occupancies;

    /// 0 - bq, 1 - bk, 2 - wq, 3 - wk
    std::bitset<4> castleRights {0ULL};

    std::optional<uint8_t> enPassantSquare = std::nullopt;

    Side::e sideToMove = Side::e::White;
};

Engine::BoardStateCache::BoardStateCache() noexcept
{
    bitboards.fill(0ULL);
    occupancies.fill(0ULL);
}

Engine::BoardStateCache::~BoardStateCache() = default;
Engine::BoardStateCache::BoardStateCache(const BoardStateCache& other) = default;

Engine::Engine() noexcept = default;
Engine::~Engine() = default;


auto Engine::generateAllPseudoLegalMoves(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const noexcept -> std::set<Move>
{
    auto moves = generatePseudoLegalMoves<PieceType::King>(boardState, side);
    moves.merge(generatePseudoLegalMoves<PieceType::Pawn>(boardState, side));
    moves.merge(generatePseudoLegalMoves<PieceType::Knight>(boardState, side));
    moves.merge(generatePseudoLegalMoves<PieceType::Queen>(boardState, side));
    moves.merge(generatePseudoLegalMoves<PieceType::Bishop>(boardState, side));
    moves.merge(generatePseudoLegalMoves<PieceType::Rook>(boardState, side));

    return moves;
}

const std::bitset<4>& Engine::getCastleRights(const std::unique_ptr<BoardStateCache>& boardState) const
{
    if (!boardState) {
        throw std::logic_error("[getCastleRights] There is no boardState!");
    }

    return boardState->castleRights;
}

auto Engine::generateAllAvailableMoves(GameBoardData&& gameBoardData) noexcept -> std::set<Move>
{
    defer([this] {
        clearCache();
    });

    updateBoardState(std::move(gameBoardData));

    return generateAllAvailableMoves(currentBoardStateCache);
}

namespace {

    /// erasing elements from set
    template <class T, class Comp, class Alloc, class Predicate>
    void erase_if(std::set<T, Comp, Alloc>& set, Predicate&& pred) {
        for (auto it{set.begin()}, end{set.end()}; it != end; ) {
            if (pred(*it)) {
                it = set.erase(it);
            } else {
                ++it;
            }
        }
    }

}   // anon namespace

auto Engine::generateAllAvailableMoves(const std::unique_ptr<BoardStateCache>& boardState) noexcept -> std::set<Move>
{
    auto pseudoLegalMoves = generateAllPseudoLegalMoves(boardState, boardState->sideToMove);

    // removing illegal moves
    std::erase_if(pseudoLegalMoves, [this, &boardState](const auto& move) {
        return !makeMove(boardState, move);
    });

    return pseudoLegalMoves;
}

void Engine::updateBoardState(GameBoardData&& gameBoardData)
{
    BoardStateCache boardState;
    boardState.castleRights = std::move(gameBoardData.castleRights);
    boardState.sideToMove = gameBoardData.sideToMove;
    boardState.enPassantSquare = gameBoardData.enPassantSquare;

    const auto& pieces = gameBoardData.pieces;

    for (const auto* piece : pieces) {
        Bitboard::Functions::setBit(boardState.bitboards[bbNumber(piece->getType(), piece->getSide())],
                piece->getPosition());
    }

    currentBoardStateCache = std::make_unique<BoardStateCache>(std::move(boardState));

    updateOccupancies(currentBoardStateCache);
}

void Engine::updateOccupancies(std::unique_ptr<BoardStateCache>& boardState)
{
    if (!boardState) {
        throw std::logic_error("[updateOccupancies] There is no boardState!");
    }

    boardState->occupancies.fill(0ULL);

    iterateOverEnum<PieceType>([&boardState](PieceType type) {
        boardState->occupancies[static_cast<uint8_t>(Side::e::White)]
                |= boardState->bitboards[bbNumber(type, Side::e::White)];
        boardState->occupancies[static_cast<uint8_t>(Side::e::Black)]
                |= boardState->bitboards[bbNumber(type, Side::e::Black)];
    });

    boardState->occupancies[static_cast<uint8_t>(Side::e::Both)] |=
            boardState->occupancies[static_cast<uint8_t>(Side::e::White)] |
            boardState->occupancies[static_cast<uint8_t>(Side::e::Black)];
}

void Engine::clearCache()
{
    currentBoardStateCache.release();
    moveResults.clear();
    pieceActionResults.clear();
}

std::optional<uint8_t> Engine::getEnPassantSquare(const std::unique_ptr<BoardStateCache>& boardState) const
{
    if (!boardState) {
        return std::nullopt;
    }

    return boardState->enPassantSquare;
}

bool Engine::isSquareAttacked(const std::unique_ptr<BoardStateCache>& boardState, uint8_t square, Side::e side) const
{
    if (!currentBoardStateCache) {
        throw std::logic_error("[isSquareAttacked] There is no boardState!");
    }

    if (side == Side::e::Both) {
        return isSquareAttacked(boardState, square, Side::e::White) || isSquareAttacked(boardState, square, Side::e::Black);
    }

    // attacked by pawns
    if (pawnAttacks[static_cast<uint8_t>(oppositeSide(side))][square]
            & getBitboard(boardState, PieceType::Pawn, side)) {
        return true;
    }

    // attacked by knights
    if (knightAttacks[square] & getBitboard(boardState, PieceType::Knight, side)) {
        return true;
    }

    // attacked by bishops
    if (getBishopAttacks(square, boardState->occupancies[static_cast<uint8_t>(Side::e::Both)]) &
            getBitboard(boardState, PieceType::Bishop, side)) {
        return true;
    }

    // attacked by rooks
    if (getRookAttacks(square, boardState->occupancies[static_cast<uint8_t>(Side::e::Both)]) &
            getBitboard(boardState, PieceType::Rook, side)) {
        return true;
    }

    // attacked by queens
    if (getQueenAttacks(square, boardState->occupancies[static_cast<uint8_t>(Side::e::Both)]) &
            getBitboard(boardState, PieceType::Queen, side)) {
        return true;
    }

    // attacked by king
    if (kingAttacks[square] & getBitboard(currentBoardStateCache, PieceType::King, side)) {
        return true;
    }

    return false;
}

auto Engine::performMove(GameBoardData&& gameBoardData, Move&& move) -> EngineResult
{
    defer([this] {
        clearCache();
    });

    updateBoardState(std::move(gameBoardData));

    if (!generateAllAvailableMoves(currentBoardStateCache).contains(move)) {
        return Error{.msg = "Illegal move"};
    }

    Result res;

    const auto& moveResult = moveResults.find(move)->second;

    res.allPossibleMoves = generateAllAvailableMoves(moveResult);
    res.pieceActionsToPerofrm = pieceActionResults.find(move)->second;
    res.sideToMove = moveResult->sideToMove;
    res.castleRights = moveResult->castleRights;
    res.enPassantSquare = moveResult->enPassantSquare;

    if (res.allPossibleMoves.empty()) {
        if (isSquareAttacked(moveResult, *Bitboard::Functions::getLSBitIndex(
            getBitboard(moveResult, PieceType::King, moveResult->sideToMove)), oppositeSide(moveResult->sideToMove))) {
            res.checkmatedSide = moveResult->sideToMove;
            res.gameStatus = GameStatus::e::Checkmate;
        } else {
            res.gameStatus = GameStatus::e::Stalemate;
        }
    }

    return res;
}

auto Engine::makeMove(const std::unique_ptr<BoardStateCache>& boardState, const Move& move) -> bool
{    
    if (!boardState) {
        throw std::logic_error("[makeMove] Invalid boardState");
    }

    const auto pieceType = move.getPieceType();
    const auto pieceSide = move.getPieceSide();
    const auto sourceSquare = move.getSourceSquare();
    const auto targetSquare = move.getTargetSquare();

    std::unique_ptr<BoardStateCache> copiedCache = std::make_unique<BoardStateCache>(*boardState);

    std::set<std::pair<SmallPieceInfoEx, PieceAction>> pieceActionsToPerofrm;

    const auto makeQuietMove = [this, &copiedCache](PieceType pieceType, Side::e side, uint8_t sourceSquare, uint8_t targetSquare) {
        Bitboard::Functions::popBit(getBitboard(copiedCache, pieceType, side), sourceSquare);
        Bitboard::Functions::setBit(getBitboard(copiedCache, pieceType, side), targetSquare);
    };

    if (isMoveCapture(move.getType())) {
        if (move.getType() == MoveType::EnPassant) {
            uint8_t squareToRemove = pieceSide == Side::e::White ? targetSquare + BOARD_SIZE : targetSquare - BOARD_SIZE;
            Bitboard::Functions::popBit(getBitboard(copiedCache, pieceType, oppositeSide(pieceSide)),
                        squareToRemove);
            pieceActionsToPerofrm.emplace(SmallPieceInfoEx{PieceType::Pawn, oppositeSide(pieceSide), targetSquare},
                                                 PieceAction::Remove);
        } else {
            iterateOverEnum<PieceType>([this, &pieceActionsToPerofrm, &copiedCache](PieceType type, Side::e side, uint8_t targetSquare) {
                if (Bitboard::Functions::popBit(getBitboard(copiedCache, type, side), targetSquare)) {
                    pieceActionsToPerofrm.emplace(SmallPieceInfoEx{type, side, targetSquare},
                                                         PieceAction::Remove);

                }
            }, oppositeSide(copiedCache->sideToMove), targetSquare);
        }
    }

    // reset enPassantSquare
    copiedCache->enPassantSquare = std::nullopt;

    if (isMovePromotion(move.getType())) {
        // remove pawn if promoted
        Bitboard::Functions::popBit(getBitboard(copiedCache, move.getPieceType(), move.getPieceSide()), move.getTargetSquare());
        Bitboard::Functions::setBit(getBitboard(copiedCache, PieceType::Queen, move.getPieceSide()), move.getTargetSquare());

        pieceActionsToPerofrm.emplace(SmallPieceInfoEx{pieceType, pieceSide, sourceSquare},
                                        PieceAction::Remove);
        pieceActionsToPerofrm.emplace(SmallPieceInfoEx{PieceType::Queen, pieceSide, targetSquare},
                                        PieceAction::Create);
    } else {
        // just move
        makeQuietMove(pieceType, pieceSide, sourceSquare, targetSquare);

        pieceActionsToPerofrm.emplace(SmallPieceInfoEx{pieceType, pieceSide, sourceSquare, targetSquare},
                                        PieceAction::Move);

        if (move.getType() == MoveType::DoublePush) {
            copiedCache->enPassantSquare = pieceSide == Side::e::White ? targetSquare + BOARD_SIZE :
                                                                         targetSquare - BOARD_SIZE;
        }

        const auto moveRookDuringCastling = [&pieceActionsToPerofrm, pieceSide, &makeQuietMove] (uint8_t oldRookPlacement, uint8_t newRookPlacement) {
            makeQuietMove(PieceType::Rook, pieceSide, oldRookPlacement, newRookPlacement);
            pieceActionsToPerofrm.emplace(SmallPieceInfoEx{PieceType::Rook, pieceSide,
                                                              oldRookPlacement, newRookPlacement},
                                            PieceAction::Move);
        };

        if (move.getType() == MoveType::Castling) {
            if (targetSquare > sourceSquare) {
                moveRookDuringCastling(sourceSquare + 3, sourceSquare + 1);
            } else {
                moveRookDuringCastling(sourceSquare - 4, sourceSquare - 1);
            }
        }


    }


    // update castling rights
    copiedCache->castleRights &= castlingRights[sourceSquare];
    copiedCache->castleRights &= castlingRights[targetSquare];

    // update side
    copiedCache->sideToMove = oppositeSide(copiedCache->sideToMove);

    // update occupancies
    updateOccupancies(copiedCache);

    // make sure king has not been exposed into a check
    if (isSquareAttacked(copiedCache, *Bitboard::Functions::getLSBitIndex(
                             getBitboard(copiedCache, PieceType::King, pieceSide)), oppositeSide(pieceSide))) {
        return false;
    } else {
        pieceActionResults.insert_or_assign(move, pieceActionsToPerofrm);
        moveResults.insert_or_assign(move, std::move(copiedCache));
        return true;
    }
}


uint64_t& Engine::getBitboard(const std::unique_ptr<BoardStateCache>& boardState, PieceType type, Side::e side) const
{
    if (!boardState) {
        throw std::logic_error("[getBitboard] There is no boardState!");
    }

    return boardState->bitboards[bbNumber(type, side)];
}

uint64_t Engine::getOccupancy(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const
{
    if (!boardState) {
        throw std::logic_error("[getOccupancy] There is no boardState!");
    }

    return boardState->occupancies[static_cast<uint8_t>(side)];
}

