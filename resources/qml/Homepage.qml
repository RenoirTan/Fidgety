import QtQuick 2.12
import QtQuick.Controls 2.12
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
    TableView {
        id: configFileList
        width: parent.width-30
        height: parent.height-100
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 25
        model: TableModel {
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
        property var columnWidths: [250, 300]
        columnWidthProvider: function (column) {
            return columnWidths[column];
        }
        delegate: Rectangle {
            implicitHeight: 100
            color: homepage.color
            border.width: 1
            border.color: "#ffffff"
            Text {
                text: display
                color: "#ffffff"
                anchors.centerIn: parent
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
        onClicked: console.log("editButton pressed")
    }
}