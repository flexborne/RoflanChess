import QtQuick 2.15

import "Constants.js" as Constants

import enum_side 1.0

Rectangle {
    id: root
    color: "transparent"

    property var availableMoves   // list of all available moves
    property var side

    function positionOfCenter() {
        return Math.floor(y/height + 1/2)*Constants.BOARD_SIZE + Math.floor(x/width + 1/2)
    }

    function updatePositionFromModel() {
        x = Math.floor(position % Constants.BOARD_SIZE)*width
        y = Math.floor(position / Constants.BOARD_SIZE)*height
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
                gameField.pieceClicked(parent.positionOfCenter(), availableMoves)
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
