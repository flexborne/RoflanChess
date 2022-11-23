import QtQuick 2.15

GridView {
    id: root

    model: 64

    cellHeight: height/8
    cellWidth: width/8

    delegate: Cell {
            id: cell

            width: root.cellWidth
            height: root.cellHeight

            Rectangle {
                id: foreground

                anchors.fill: parent

                opacity: 0.8
                color: "#6495ed"
                visible: false

                Connections {
                    target: gameField

                    function onPieceClicked(availableMoves) {
                        if (isMoveAvailable(index, availableMoves)) {
                            foreground.visible = true
                        }
                    }

                    function onPieceReleased() {
                        foreground.visible = false
                    }
                }
            }
        }


    interactive: false
}
