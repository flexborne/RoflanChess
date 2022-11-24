import QtQuick 2.15
import QtMultimedia 6.3

import "Constants.js" as Constants

import enum_gameStatus 1.0


Item {
    property var sideToMove
    property var gameStatus

    function fetchGamePropertiesFromModel() {
        sideToMove = boardModel.getSideToMove()
        gameStatus = boardModel.getGameStatus()
    }


    Component.onCompleted: {
        fetchGamePropertiesFromModel()
    }

    SoundEffect {
        id: victory_song
        source: Constants.VICTORY_SONG_PATH
    }

    Connections {
        target: boardModel
        function onGamePropertiesUpdated() {
            fetchGamePropertiesFromModel()
            if (gameStatus === GameStatus.Checkmate) {
                victory_song.play()
            }
        }
    }

    ChessBoard { }
}
