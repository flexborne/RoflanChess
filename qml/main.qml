import QtQuick 2.15
import QtQuick.Window 2.15

import "Constants.js" as Constants


Window {
    id: mainWindow
    width: Constants.BOARD_WIDTH
    height: Constants.BOARD_HEIGHT
    visible: true
    title: qsTr("Roflan Chess")

    Game { }
}
