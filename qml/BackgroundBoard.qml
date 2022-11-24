import QtQuick 2.15

import "Constants.js" as Constants

GridView {
    id: root

    model: Constants.N_CELLS

    cellHeight: height/Constants.BOARD_SIZE
    cellWidth: width/Constants.BOARD_SIZE

    delegate: Cell {
            id: cell

            width: root.cellWidth
            height: root.cellHeight

            Rectangle {
                id: highlight

                anchors.fill: parent

                opacity: Constants.BACKGROUND_OPACITY
                color: Constants.DEFAULT_HIGHLIGHT_COLOR
                visible: false

                Connections {
                    target: gameField

                    function onPieceClicked(sourceSquare, availableMoves) {
                        if (isMoveAvailable(index, availableMoves)) {
                            highlight.visible = true
                        }

                        if (index === sourceSquare) {
                            highlight.color = Constants.SOURCE_SQ_HIGHLIGHT_COLOR
                            highlight.visible = true

                        }
                    }

                    function onPieceReleased() {
                        highlight.color = Constants.DEFAULT_HIGHLIGHT_COLOR
                        highlight.visible = false
                    }
                }
            }
        }


    interactive: false
}
