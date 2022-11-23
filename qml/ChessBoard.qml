import QtQuick 2.15

Item {
    id: gameField


    signal pieceClicked(var availableMoves)
    signal pieceReleased()

    /// @return move, if it's available
    function isMoveAvailable(move, availableMoves) {
        for (var idx in availableMoves) {
            if (move === availableMoves[idx].target_square) {
                return availableMoves[idx]
            }
        }
        return null
    }

    BackgroundBoard {
        id: board

        //anchors.fill: parent
        width: 700
        height: 700
    }

    PiecesBoard {
        id: piecesBoard

        anchors.fill: board
    }
}
