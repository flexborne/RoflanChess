#include "game.h"

#include "boardManager.h"
#include "boardModel.h"

Game::Game() noexcept : boardManager{std::make_shared<BoardManager>()}, boardModel {std::make_shared<BoardModel>(boardManager)} {}

auto Game::getBoardModel() const noexcept -> const std::shared_ptr<BoardModel>&
{
    return boardModel;
}

void Game::setBoardModel(const auto& boardModel)
{
    this->boardModel = boardModel;
}

auto Game::getBoard() const noexcept -> const std::shared_ptr<BoardManager>&
{
    return boardManager;
}

void Game::setBoard(const auto& boardData)
{
    this->boardManager = boardData;
}
