import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.labs.qmlmodels 1.0

ApplicationWindow {
    id: homepage
    visible: true
    minimumWidth: 640
    minimumHeight: 360
    width: 800
    height: 450
    title: "Fidgety"
    Rectangle {
        id: cflBox
        width: parent.width-20
        height: parent.height-50
        border.width: 1
        border.color: "#7f7f7f"
        color: homepage.color
        TableView {
            id: cflTable
            anchors.fill: parent
            visible: true
            clip: true
            model: ConfigFilesModel {}
            ScrollBar.horizontal: ScrollBar {}
            ScrollBar.vertical: ScrollBar {}
            ScrollIndicator.horizontal: ScrollIndicator { }
            ScrollIndicator.vertical: ScrollIndicator { }
            property int selectedRow: 0
            property var columnWidths: [250, 300]
            property var columnNames: ["Name", "Path"]
            columnWidthProvider: function (column) { return cflTable.width/2; }
            rowHeightProvider: function (row) { return 30; }
            Row {
                id: cflTableHeader
                y: cflTable.contentY
                z: 2
                Repeater {
                    model: 2
                    Label {
                        width: cflTable.columnWidthProvider(modelData)
                        height: 30
                        text: cflTable.columnNames[index]
                        padding: 10
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "white"
                        background: Rectangle { color: "#7f7f7f" }
                    }
                }
            }
            delegate: DelegateChooser {
                // Name
                DelegateChoice {
                    column: 0
                    delegate: Text {
                        padding: 10
                        width: cflTable.columnWidthProvider(0)
                        color: "white"
                        text: model.display
                    }
                }
                // Path
                DelegateChoice {
                    column: 1
                    delegate: Text {
                        padding: 10
                        width: cflTable.columnWidthProvider(1)
                        color: "white"
                        text: model.display
                    }
                }
            }
        }
    }
    Button {
        id: editButton
        text: "Edit"
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: cflBox.bottom
        anchors.topMargin: 0
        anchors.bottomMargin: 10
        onClicked: console.log("editButton pressed")
    }
}