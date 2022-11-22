#ifndef GAME_H
#define GAME_H

// std
#include <memory>

class BoardModel;
class BoardManager;

class Game
{
public:
    explicit Game() noexcept;

    auto getBoardModel() const noexcept -> const std::shared_ptr<BoardModel>&;
    void setBoardModel(const auto& boardModel);

    auto getBoard() const noexcept -> const std::shared_ptr<BoardManager>&;
    void setBoard(const auto& board);

private:
    std::shared_ptr<BoardManager> boardManager;
    std::shared_ptr<BoardModel> boardModel;
};

#endif // GAME_H
