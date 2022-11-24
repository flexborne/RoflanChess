import QtQuick 2.15

import "Constants.js" as Constants

Rectangle {
    id: root

    color: {
        var row = Math.floor(index / Constants.BOARD_SIZE);
        var column = index % Constants.BOARD_SIZE

        if ((row + column) % 2 == 1)
            Constants.CELL_DARK_COLOR;
        else
            Constants.CELL_LIGHT_COLOR;
    }


}
