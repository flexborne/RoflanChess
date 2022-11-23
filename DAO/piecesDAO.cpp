#include "piecesDAO.h"

// std
#include <ranges>


void PiecesDAO::create(std::unique_ptr<Piece>&& piece)
{
    pieces.emplace_back(std::move(piece));
}

void PiecesDAO::update(uint8_t idx, std::unique_ptr<Piece>&& piece)
{
    pieces[idx] = std::move(piece);
}

const Piece* PiecesDAO::get(uint8_t idx) const
{
    if (idx < pieces.size()) {
        return pieces[idx].get();
    }
    throw std::invalid_argument("[BoardDAO get] Invalid index");
}

auto PiecesDAO::getAll() const -> std::vector<const Piece*>
{
    std::vector<const Piece*> transformedPieces;

    for (const auto& i : pieces) {
        transformedPieces.emplace_back(i.get());
    }

    return transformedPieces;
}

auto PiecesDAO::getByPredicate(std::function<bool(std::unique_ptr<Piece>&)> predicate) -> std::optional<Piece *>
{
    if (auto result = std::ranges::find_if(pieces, predicate); result != pieces.end()) {
        return (*result).get();
    }

    return std::nullopt;
}

void PiecesDAO::remove(uint8_t idx)
{
    if (idx < pieces.size()) {
        pieces.erase(std::next(pieces.begin(), idx));
    }
    throw std::invalid_argument("[BoardDAO remove] Invalid index");
}

void PiecesDAO::removeByPredicate(std::function<bool(std::unique_ptr<Piece>&)> predicate)
{
    std::erase_if(pieces, predicate);
}

void PiecesDAO::applyFunc(std::function<void(std::unique_ptr<Piece>&)> func)
{
    std::ranges::for_each(pieces, func);
}
