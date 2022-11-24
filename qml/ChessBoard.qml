import QtQuick 2.15

import "Constants.js" as Constants

Item {
    id: gameField


    signal pieceClicked(var sourceSquare, var availableMoves)
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

        width: Constants.BOARD_WIDTH
        height: Constants.BOARD_HEIGHT
    }

    PiecesBoard {
        id: piecesBoard

        anchors.fill: board
    }
}
