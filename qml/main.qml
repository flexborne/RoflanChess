import QtQuick 2.15
import QtQuick.Window 2.15
import QtMultimedia 6.3

import enum_gameStatus 1.0


Window {
    width: 700
    height: 700
    visible: true
    title: qsTr("Roflan Chess")


    property var sideToMove: boardModel.getSideToMove()
    property var gameStatus: boardModel.getGameStatus()

    SoundEffect {
        id: victory_song
        source: "qrc:/sounds/victorySong.wav"

    }

    Connections {
        target: boardModel
        function onSideToMoveUpdated() {
            sideToMove = boardModel.getSideToMove()
            gameStatus = boardModel.getGameStatus()
            if (gameStatus === GameStatus.Checkmate) {
                victory_song.play()
            }
        }
    }

    ChessBoard {
//        width: 700
//        height: 700
    }
}
