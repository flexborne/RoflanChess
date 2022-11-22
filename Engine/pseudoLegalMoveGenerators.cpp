#include "bitboard.h"
#include "engine.h"
#include "engineDefines.h"
#include "move.h"

/// @brief implementations of pseudo legal move generators

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

/// @brief generating pawn moves
template <>
auto Engine::generatePseudoLegalMoves<PieceType::Pawn>(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const -> std::set<Move>
{
    if (side == Side::e::Both) {
        auto bothSideMoves = generatePseudoLegalMoves<PieceType::Pawn>(boardState, Side::e::White);
        bothSideMoves.merge(generatePseudoLegalMoves<PieceType::Pawn>(boardState, Side::e::Black));
        return bothSideMoves;
    }

    std::set<Move> pawnMoves;

    auto bitboard = getBitboard(boardState, PieceType::Pawn, side);

    const auto isPawnOnInitialRank = [](uint8_t sourceSquare, Side::e side) noexcept -> bool {
        switch (side) {
            case Side::e::White:
                return sourceSquare >= 6*BOARD_SIZE && sourceSquare < 7*BOARD_SIZE;
            case Side::e::Black:
                return sourceSquare >= BOARD_SIZE && sourceSquare < 2*BOARD_SIZE;
            default:
                return false;
        }
    };

    const auto isPawnOnPromotionRank = [&isPawnOnInitialRank](uint8_t sourceSquare, Side::e side) noexcept -> bool {
        return isPawnOnInitialRank(sourceSquare, oppositeSide(side));
    };

    const auto getQuietMoves = [&](uint8_t sourceSquare, Side::e side) noexcept -> std::set<Move> {
        std::set<Move> quietMoves;

        if (int8_t targetSquare = side == Side::e::White ? sourceSquare - BOARD_SIZE :
                sourceSquare + BOARD_SIZE; !(targetSquare < 0) && !(targetSquare > N_SQUARES - 1) &&
                !Bitboard::Functions::getBit(getOccupancy(boardState, Side::e::Both),
               targetSquare)) {
            // quiet promotion move
            if (isPawnOnPromotionRank(sourceSquare, side)) {
                quietMoves.emplace(Move{PieceType::Pawn, side, sourceSquare, static_cast<uint8_t>(targetSquare), MoveType::QuietPromotion});
            } else {
                // quiet move
                quietMoves.emplace(Move{PieceType::Pawn, side, sourceSquare, static_cast<uint8_t>(targetSquare), MoveType::Quiet});

                // double pawn push
                if (isPawnOnInitialRank(sourceSquare, side)) {
                    if (uint8_t doublePushSquare = side == Side::e::White ? targetSquare - BOARD_SIZE :
                            targetSquare + BOARD_SIZE;
                            !Bitboard::Functions::getBit(getOccupancy(boardState, Side::e::Both),
                                doublePushSquare)) {
                        quietMoves.emplace(Move{PieceType::Pawn, side, sourceSquare, doublePushSquare, MoveType::DoublePush});
                    }
                }

            }
        }

        return quietMoves;
    };


    uint8_t sourceSquare, targetSquare;

    // generate pawns moves
    while (bitboard) {
        // init source square
        sourceSquare = Bitboard::Functions::getLSBitIndex(bitboard).value_or(0);

        pawnMoves.merge(getQuietMoves(sourceSquare, side));

        auto attacks = pawnAttacks[static_cast<uint8_t>(side)][sourceSquare]
                & getOccupancy(boardState, oppositeSide(side));

        // generate pawn captures
        while (attacks) {
            targetSquare = Bitboard::Functions::getLSBitIndex(attacks).value_or(0);

            // pawn capture promotions
            if (isPawnOnPromotionRank(sourceSquare, side)) {
                pawnMoves.emplace(PieceType::Pawn, side, sourceSquare, targetSquare, MoveType::CapturePromotion);
            } else {    // pawn capture
                pawnMoves.emplace(PieceType::Pawn, side, sourceSquare, targetSquare, MoveType::Capture);
            }

            // pop targetSquare
            Bitboard::Functions::popBit(attacks, targetSquare);
        }

        // enPassant move
        if (auto enPassantSquareOpt = getEnPassantSquare(boardState)) {
            if (auto enPassantAttacks = pawnAttacks[static_cast<uint8_t>(side)][sourceSquare]
                    & (1ULL << *enPassantSquareOpt)) {
                pawnMoves.emplace(Move{PieceType::Pawn, side, sourceSquare,
                        Bitboard::Functions::getLSBitIndex(enPassantAttacks).value_or(0),
                        MoveType::EnPassant});
            }
        }

        // pop sourceSquare
        Bitboard::Functions::popBit(bitboard, sourceSquare);
    }

    return pawnMoves;
}

/// @brief generating king moves
template <>
auto Engine::generatePseudoLegalMoves<PieceType::King>(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const -> std::set<Move>
{
    if (side == Side::e::Both) {
        auto bothSideMoves = generatePseudoLegalMoves<PieceType::King>(boardState, Side::e::White);
        bothSideMoves.merge(generatePseudoLegalMoves<PieceType::King>(boardState, Side::e::Black));
        return bothSideMoves;
    }

    std::set<Move> moves;

    uint8_t kingPlacement;

    if (auto kingPlacementOpt = Bitboard::Functions::getLSBitIndex(getBitboard(boardState, PieceType::King, side))) {
         kingPlacement = *kingPlacementOpt;
    } else {
        throw std::logic_error("[generatePseudoLegalMoves] There is no king!");
    }

    // castling moves
    const auto& castleRights = getCastleRights(boardState);

    // king side
    if ((castleRights[1] && side == Side::e::White) || (castleRights[3] && side == Side::e::Black)) {
        // check whether squares between rook and king are occupied & king is not under attack
        if (!Bitboard::Functions::getBit(getOccupancy(boardState, Side::e::Both), kingPlacement + 1)
                && !Bitboard::Functions::getBit(getOccupancy(boardState, Side::e::Both), kingPlacement + 2)
                && !isSquareAttacked(boardState, kingPlacement, oppositeSide(side))) {
                moves.emplace(PieceType::King, side, kingPlacement, kingPlacement + 2, MoveType::Castling);
        }
    }

    // queen side
    if ((castleRights[0] && side == Side::e::White) || (castleRights[2] && side == Side::e::Black)) {
        if (!Bitboard::Functions::getBit(getOccupancy(boardState, Side::e::Both), kingPlacement - 1)
                && !Bitboard::Functions::getBit(getOccupancy(boardState, Side::e::Both), kingPlacement - 2)
                && !Bitboard::Functions::getBit(getOccupancy(boardState, Side::e::Both), kingPlacement - 3)
                && !isSquareAttacked(boardState, kingPlacement, oppositeSide(side))) {
                moves.emplace(PieceType::King, side, kingPlacement, kingPlacement - 2, MoveType::Castling);
        }
    }


    // king attacks
    auto attacks = kingAttacks[kingPlacement] & ~getOccupancy(boardState, side);

    while (attacks) {
        auto targetSquare = Bitboard::Functions::getLSBitIndex(attacks).value_or(0);

        // quiet move
        if (!Bitboard::Functions::getBit(getOccupancy(boardState, oppositeSide(side)), targetSquare)) {
            moves.emplace(PieceType::King, side, kingPlacement, targetSquare, MoveType::Quiet);
        } else {    // capture move
            moves.emplace(PieceType::King, side, kingPlacement, targetSquare, MoveType::Capture);
        }

        // pop targetSquare
        Bitboard::Functions::popBit(attacks, targetSquare);
    }

    return moves;
}

/// @brief generating knight moves
template <>
auto Engine::generatePseudoLegalMoves<PieceType::Knight>(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const -> std::set<Move>
{
    if (side == Side::e::Both) {
        auto bothSideMoves = generatePseudoLegalMoves<PieceType::Knight>(boardState, Side::e::White);
        bothSideMoves.merge(generatePseudoLegalMoves<PieceType::Knight>(boardState, Side::e::Black));
        return bothSideMoves;
    }
    std::set<Move> moves;

    auto bitboard = getBitboard(boardState, PieceType::Knight, side);

    while (bitboard) {
        auto sourceSquare = Bitboard::Functions::getLSBitIndex(bitboard).value_or(0);

        auto attacks = knightAttacks[sourceSquare] & ~getOccupancy(boardState, side);

        while (attacks) {
            auto targetSquare = Bitboard::Functions::getLSBitIndex(attacks).value_or(0);

            // quiet move
            if (!Bitboard::Functions::getBit(getOccupancy(boardState, oppositeSide(side)), targetSquare)) {
                moves.emplace(PieceType::Knight, side, sourceSquare, targetSquare, MoveType::Quiet);
            } else {    // capture move
                moves.emplace(PieceType::Knight, side, sourceSquare, targetSquare, MoveType::Capture);
            }

            // pop targetSquare
            Bitboard::Functions::popBit(attacks, targetSquare);
        }

        // pop sourceSquare
        Bitboard::Functions::popBit(bitboard, sourceSquare);
    }

    return moves;
}

/// @brief generating bishop moves
template <>
auto Engine::generatePseudoLegalMoves<PieceType::Bishop>(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const -> std::set<Move>
{
    if (side == Side::e::Both) {
        auto bothSideMoves = generatePseudoLegalMoves<PieceType::Bishop>(boardState, Side::e::White);
        bothSideMoves.merge(generatePseudoLegalMoves<PieceType::Bishop>(boardState, Side::e::Black));
        return bothSideMoves;
    }

    std::set<Move> moves;

    auto bitboard = getBitboard(boardState, PieceType::Bishop, side);

    while (bitboard) {
        auto sourceSquare = Bitboard::Functions::getLSBitIndex(bitboard).value_or(0);

        auto attacks = getBishopAttacks(sourceSquare, getOccupancy(boardState, Side::e::Both))
                & ~getOccupancy(boardState, side);

        while (attacks) {
            auto targetSquare = Bitboard::Functions::getLSBitIndex(attacks).value_or(0);

            // quiet move
            if (!Bitboard::Functions::getBit(getOccupancy(boardState, oppositeSide(side)), targetSquare)) {
                moves.emplace(PieceType::Bishop, side, sourceSquare, targetSquare, MoveType::Quiet);
            } else {    // capture move
                moves.emplace(PieceType::Bishop, side, sourceSquare, targetSquare, MoveType::Capture);
            }

            // pop targetSquare
            Bitboard::Functions::popBit(attacks, targetSquare);
        }

        // pop sourceSquare
        Bitboard::Functions::popBit(bitboard, sourceSquare);
    }

    return moves;
}

/// @brief generating rook moves
template <>
auto Engine::generatePseudoLegalMoves<PieceType::Rook>(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const -> std::set<Move>
{
    if (side == Side::e::Both) {
        auto bothSideMoves = generatePseudoLegalMoves<PieceType::Rook>(boardState, Side::e::White);
        bothSideMoves.merge(generatePseudoLegalMoves<PieceType::Rook>(boardState, Side::e::Black));
        return bothSideMoves;
    }

    std::set<Move> moves;

    auto bitboard = getBitboard(boardState, PieceType::Rook, side);

    while (bitboard) {
        auto sourceSquare = Bitboard::Functions::getLSBitIndex(bitboard).value_or(0);

        auto attacks = getRookAttacks(sourceSquare, getOccupancy(boardState, Side::e::Both))
                & ~getOccupancy(boardState, side);

        while (attacks) {
            auto targetSquare = Bitboard::Functions::getLSBitIndex(attacks).value_or(0);

            // quiet move
            if (!Bitboard::Functions::getBit(getOccupancy(boardState, oppositeSide(side)), targetSquare)) {
                moves.emplace(PieceType::Rook, side, sourceSquare, targetSquare, MoveType::Quiet);
            } else {    // capture move
                moves.emplace(PieceType::Rook, side, sourceSquare, targetSquare, MoveType::Capture);
            }

            // pop targetSquare
            Bitboard::Functions::popBit(attacks, targetSquare);
        }

        // pop sourceSquare
        Bitboard::Functions::popBit(bitboard, sourceSquare);
    }

    return moves;
}

/// @brief generating queen moves
template <>
auto Engine::generatePseudoLegalMoves<PieceType::Queen>(const std::unique_ptr<BoardStateCache>& boardState, Side::e side) const -> std::set<Move>
{
    if (side == Side::e::Both) {
        auto bothSideMoves = generatePseudoLegalMoves<PieceType::Queen>(boardState, Side::e::White);
        bothSideMoves.merge(generatePseudoLegalMoves<PieceType::Queen>(boardState, Side::e::Black));
        return bothSideMoves;
    }

    std::set<Move> moves;

    auto bitboard = getBitboard(boardState, PieceType::Queen, side);

    while (bitboard) {
        auto sourceSquare = Bitboard::Functions::getLSBitIndex(bitboard).value_or(0);

        auto attacks = getQueenAttacks(sourceSquare, getOccupancy(boardState, Side::e::Both))
                & ~getOccupancy(boardState, side);

        while (attacks) {
            auto targetSquare = Bitboard::Functions::getLSBitIndex(attacks).value_or(0);

            // quiet move
            if (!Bitboard::Functions::getBit(getOccupancy(boardState, oppositeSide(side)), targetSquare)) {
                moves.emplace(PieceType::Queen, side, sourceSquare, targetSquare, MoveType::Quiet);
            } else {    // capture move
                moves.emplace(PieceType::Queen, side, sourceSquare, targetSquare, MoveType::Capture);
            }

            // pop targetSquare
            Bitboard::Functions::popBit(attacks, targetSquare);
        }

        // pop sourceSquare
        Bitboard::Functions::popBit(bitboard, sourceSquare);
    }

    return moves;
}
