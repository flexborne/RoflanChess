#ifndef BOARDDAO_H
#define BOARDDAO_H

#include "DAO.h"

#include "piece.h"

// std
#include <memory>
#include <vector>

class Piece;

template <class T, class Return_T = T>
class I_Searchable
{
public:
    virtual auto getByPredicate(std::function<bool(T&)> predicate) -> Return_T = 0;
};

/// @brief DAO, that uses predicates
template <class DTO>
class Predicate_DAO
{
    using predicate_T = std::function<bool(DTO&)>;
public:
    virtual void applyFunc(std::function<void(DTO&)> func) = 0;
    virtual void removeByPredicate(predicate_T predicate)  = 0;
};

/// filterable DAO
class PiecesDAO : DAO<std::unique_ptr<Piece>>,
                 I_Searchable<std::unique_ptr<Piece>, std::optional<Piece*>>,
                 Predicate_DAO<std::unique_ptr<Piece>>
{
    using Base = DAO<std::unique_ptr<Piece>>;
public:
    PiecesDAO() noexcept = default;

    // DAO interface
public:
    void create(std::unique_ptr<Piece>&& piece) final;

    void update(uint8_t idx, std::unique_ptr<Piece>&& piece) final;

    const Piece* get(uint8_t idx) const final;

    auto getAll() const -> std::vector<const Piece*> final;

    auto getByPredicate(std::function<bool(std::unique_ptr<Piece>&)> predicate)
        -> std::optional<Piece*> final;

    void remove(uint8_t idx) final;

    void removeByPredicate(std::function<bool(std::unique_ptr<Piece>&)> predicate) final;

    void applyFunc(std::function<void(std::unique_ptr<Piece>&)> predicate) final;

private:
    std::vector<std::unique_ptr<Piece>> pieces;
};

#endif // BOARDDAO_H
