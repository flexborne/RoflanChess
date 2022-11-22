import QtQuick 2.15

Rectangle {
    height: 70
    width: 70

    id: root

    color: {
        var row = Math.floor(index / 8);
        var column = index % 8

        if ((row + column) % 2 == 1)
            "#8B4513";
        else
            "#FFE4B5";
    }


}
