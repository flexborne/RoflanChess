#include "boardModel.h"

#include "boardManager.h"

// game
#include "piece.h"
#include "move.h"

// utils
#include "u_defer.h"


const QHash<int, QByteArray> BoardModel::roleLongNames {
    { ChessBoardModelRoles::PositionRole, "position" },
    { ChessBoardModelRoles::ImagePathRole, "image_path" },
    { ChessBoardModelRoles::AvailableMovesRole, "available_moves" },
    { ChessBoardModelRoles::SideRole, "side_of_piece" },
    { ChessBoardModelRoles::PerformedMoveRole, "performed_move" }
};

BoardModel::BoardModel(std::shared_ptr<BoardManager> boardManager, QObject* parent) noexcept :
    BaseModel{parent}, boardManager{boardManager} { }

QHash<int, QByteArray> BoardModel::roleNames() const
{
    return roleLongNames;
}

int BoardModel::rowCount([[maybe_unused]] const QModelIndex& parent) const
{
    return boardManager->getPieces().size();
}

namespace {
    template <template<class, class...> class Container, class Elem, class... Params, class OP = std::identity>
    auto containerToQVariantList(const Container<Elem, Params...>& container, OP op = {}) -> QVariantList
    {
        QVariantList list;

        std::ranges::for_each(container, [&list, &op](const Elem& element)
        {
            list.append(QVariant::fromValue(std::invoke(op, element)));
        });

        return list;
    }
}

QVariant BoardModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() ) {
        return QVariant {};
    }

    const auto* piece = boardManager->getPieceAt(index.row());

    switch (role) {
        case PositionRole:
            return QVariant::fromValue(piece->getPosition());
        case ImagePathRole:
            return QVariant::fromValue(piece->getImagePath());
        case SideRole:
            return QVariant::fromValue(piece->getSide());
        case AvailableMovesRole:
            return containerToQVariantList(piece->getAvailableMoves());
    }

    return QVariant {};
}


bool BoardModel::setData(const QModelIndex&, const QVariant& value, int role)
{
    if(!boardManager)
        return false;

    switch (role) {
        case PerformedMoveRole: {
            beginResetModel();
            defer([this] {
                endResetModel();
                emit sideToMoveUpdated();
            });

            boardManager->performMove(qvariant_cast<Move>(value));
            return true;
        }
        default: return false;
    }

    return false;
}

void BoardModel::setBoardManager(const std::shared_ptr<BoardManager>& boardManager)
{
    beginResetModel();
    defer([this] {
        endResetModel();
    });

    this->boardManager = boardManager;
}

auto BoardModel::getSideToMove() const -> Side::e
{
    return boardManager->getSideToMove();
}

auto BoardModel::getGameStatus() const -> GameStatus::e
{
    return boardManager->getGameStatus();
}


