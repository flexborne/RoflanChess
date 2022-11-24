import QtQuick 2.15

import "Constants.js" as Constants

Repeater {
    id: root

    model: boardModel

    delegate: PieceDelegate {
        id: piece

        width: root.width/Constants.BOARD_SIZE
        height: root.height/Constants.BOARD_SIZE

        Component.onCompleted: {
            piece.updatePositionFromModel()
            piece.availableMoves = available_moves
            piece.side = side_of_piece
        }
    }
}
