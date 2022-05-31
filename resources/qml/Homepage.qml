import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4
import Qt.labs.qmlmodels 1.0

ApplicationWindow {
    id: homepage
    visible: true
    minimumWidth: 640
    minimumHeight: 480
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
    Rectangle {
        id: cflBox
        width: parent.width-30
        height: parent.height-100
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        color: parent.color
        TableView {
                anchors.fill: parent
                clip: true
            TableViewColumn {
                role: "name"
                title: "Name"
            }
            TableViewColumn {
                role: "path"
                title: "Path"
            }
            model: TableModel {
                id: cflModel
                TableModelColumn { display: "name" }
                TableModelColumn { display: "path" }
                rows: [
                    {
                        name: "GRand Unified Bootloader",
                        path: "/etc/default/grub"
                    },
                    {
                        name: "TLP",
                        path: "/etc/tlp.conf"
                    }
                ]
            }
        }
    }
    Button {
        id: editButton
        text: "Edit"
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: cflBox.bottom
        anchors.topMargin: 10
        onClicked: console.log("editButton pressed")
    }
}