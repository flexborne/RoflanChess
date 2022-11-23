#include "boardManager.h"

#include "piece.h"

// pieces creation
#include "factory.h"
#include "fenParser.h"

#include "gameBoardData.h"

// std
#include <ranges>

#include "error.h"
#include "u_overloaded.h"


BoardManager::BoardManager() noexcept :
    engine{std::make_unique<Engine>()}
{
    fetchFenInfo();
    updatePiecesAvailableMoves(engine->generateAllAvailableMoves(buildGameBoardDTO()));
}

void BoardManager::fetchFenInfo() noexcept
{
    auto parseResult = FenParser::parseFen(/*"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2b2Q1p/PPPbBPPP/R3K2R w KQkq - 0 1 "*/);
    for (auto& pieceInfo : parseResult.piecesToCreate) {
        piecesDAO.create(Factory::create(std::move(pieceInfo)));
    }

    castleRights = parseResult.castleRights;
    enPassantSquare = parseResult.enPassantSquare;
    sideToMove = parseResult.toMove;
}

auto BoardManager::getPieces() const -> std::vector<const Piece*>
{
    return piecesDAO.getAll();
}

auto BoardManager::getPieceAt(uint8_t index) const  -> const Piece*
{
    return piecesDAO.get(index);
}

auto BoardManager::getSideToMove() const -> Side::e
{
    return sideToMove;
}

auto BoardManager::getGameStatus() const -> GameStatus::e
{
    return gameStatus;
}


void BoardManager::performMove(Move&& move)
{
    handleEngineResult(engine->performMove(buildGameBoardDTO(), std::move(move)));
}

void BoardManager::handleEngineResult(Engine::EngineResult&& engineResult)
{
    std::visit(utils::Overloaded{
        [](Error&& error) {
            throw std::invalid_argument(error.msg);
        },
        [this](Engine::Result&& engineResult) {
            for (auto& [pieceInfo, action] : engineResult.pieceActionsToPerofrm) {
                switch (action) {
                    case Engine::PieceAction::Move: {
                        auto neededPiece = piecesDAO.getByPredicate([position = pieceInfo.position](const auto& piece) {
                            if (position == piece->getPosition())
                                return true;
                            return false;
                        });

                        if (!neededPiece.has_value()) {
                            throw std::invalid_argument("[BoardManager handleEngineResult] Invalid move");
                        }

                        if (!pieceInfo.movedTo.has_value()) {
                            throw std::invalid_argument("[BoardManager handleEngineResult] Invalid pieceInfo");
                        }

                        (*neededPiece)->setPosition(*pieceInfo.movedTo);
                        break;
                     }
                     case Engine::PieceAction::Create: {
                        piecesDAO.create(Factory::create(std::move(pieceInfo)));
                        break;
                     }
                     case Engine::PieceAction::Remove: {
                        piecesDAO.removeByPredicate([position = pieceInfo.position, side = pieceInfo.side] (const auto& piece){
                            return position == piece->getPosition() && side == piece->getSide();
                        });
                        break;
                     }
                }
            }

            updatePiecesAvailableMoves(engineResult.allPossibleMoves);

            sideToMove = engineResult.sideToMove;
            castleRights = engineResult.castleRights;
            enPassantSquare = engineResult.enPassantSquare;

            checkmatedSide = engineResult.checkmatedSide;
            gameStatus = engineResult.gameStatus;
        }},
        std::move(engineResult));
}

void BoardManager::updatePiecesAvailableMoves(const std::set<Move>& availableMoves)
{
    // clearing all possible moves for every piece
    piecesDAO.applyFunc([](const auto& piece) {
        piece->clearMoves();
    });

    // adding moves
    std::ranges::for_each(availableMoves,
        [this](Move move) {
            auto neededPiece = piecesDAO.getByPredicate([&move](const auto& piece) {
                if (move.getSourceSquare() == piece->getPosition())
                    return true;
                return false;
            });

            if (!neededPiece.has_value()) {
                throw std::invalid_argument("[BoardManager handleEngineResult] Invalid move");
            }

            (*neededPiece)->addAvailableMove(move);
        }
    );
}

auto BoardManager::buildGameBoardDTO() const -> GameBoardData
{
    return GameBoardData{.pieces = piecesDAO.getAll(),
                .sideToMove = sideToMove,
                .castleRights = castleRights,
                .enPassantSquare = enPassantSquare};
}

