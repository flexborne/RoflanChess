#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <QAbstractListModel>

#include "defines.h"
#include "enum_GameStatus.h"

// std
#include <memory>
#include <vector>

class BoardManager;

class BoardModel : public QAbstractListModel
{
    Q_OBJECT

private:
    using BaseModel = QAbstractListModel;

public:
    explicit BoardModel(std::shared_ptr<BoardManager> board, QObject* parent = nullptr) noexcept;

    QHash<int, QByteArray> roleNames() const final;

    int rowCount(const QModelIndex& parent = QModelIndex {}) const final;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const final;

    bool setData(const QModelIndex& index, const QVariant& value, int role) final;

    void setBoardManager(const std::shared_ptr<BoardManager>& boardManager);

    Q_INVOKABLE Side::e getSideToMove() const;
    Q_INVOKABLE GameStatus::e getGameStatus() const;

signals:
    void sideToMoveUpdated();

private:
    enum ChessBoardModelRoles {
        PositionRole = Qt::UserRole + 1,
        ImagePathRole,
        AvailableMovesRole,
        SideRole,
        PerformedMoveRole
    };

    static const QHash<int, QByteArray> roleLongNames;

    std::shared_ptr<BoardManager> boardManager;

};

#endif // BOARDMODEL_H
