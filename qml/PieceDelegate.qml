import QtQuick 2.15

import enum_side 1.0

Rectangle {
    id: root
    color: "transparent"

    property var availableMoves   // list of all available moves
    property var side

    function positionOfCenter() {
        return Math.floor(y/height + 1/2)*8 + Math.floor(x/width + 1/2)
    }

    function updatePositionFromModel() {
        x = Math.floor(position % 8)*width
        y = Math.floor(position / 8)*height
    }

    Image {
        id: image

        anchors.fill: root
        source: image_path
    }

    MouseArea {
        id: mouseArea

        anchors.fill: parent
        drag.target: (parent.side === sideToMove) ? parent : null

        onPressed: {
            parent.z = 1    // focus

            if (parent.side === sideToMove) {
                gameField.pieceClicked(availableMoves)
            }

        }

        onReleased: {
            parent.z = 0
            gameField.pieceReleased()

            let targetSquare = isMoveAvailable(parent.positionOfCenter(), parent.availableMoves)
            if (targetSquare) {
                performed_move = targetSquare
            } else {
                // resetting position in case move is not available
                parent.updatePositionFromModel()
            }
        }
    }

}
