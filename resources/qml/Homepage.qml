import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id: homepage
    visible: true
    width: 640
    height: 480
    title: "Fidgety"
    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem {
                text: "Exit"
                onTriggered: console.log("Not implemented")
            }
        }
    }
    ListView {
        id: configFileList
        width: parent.width-30
        height: parent.height-100
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 25
        model: ConfigFilesModel {}
        ButtonGroup {
            id: configFileListButtons
        }
        delegate: Item {
            width: parent.width-20
            height: 30
            RadioButton {
                text: name + ": " + path
                // anchors.right: parent.right
                onClicked: console.log("User has selected '" + path + "'")
                ButtonGroup.group: configFileListButtons
            }
        }
    }
    Button {
        id: editButton
        text: "Edit"
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: configFileList.bottom
        anchors.topMargin: 10
    }
}