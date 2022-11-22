import QtQuick 2.15

Repeater {
    id: root

    model: boardModel


    delegate: PieceDelegate {
        id: piece

        width: root.width/8
        height: root.height/8

        Component.onCompleted: {
            piece.updatePositionFromModel()
            piece.availableMoves = available_moves
            piece.side = side_of_piece
        }
    }
}
