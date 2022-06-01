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
    TableView {
        id: cflTable
        width: parent.width-30
        height: parent.height-100
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        property int selectedRow: 0
        TableViewColumn {
            role: "name"
            title: "Name"
        }
        TableViewColumn {
            role: "path"
            title: "Path"
        }
        // columnWidthProvider: function (column) { return [250, 300][column]; }
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
                    name: "Linux Advanced Power Management",
                    path: "/etc/tlp.conf"
                },
                {
                    name: "Xorg",
                    path: "/etc/X11/xorg.conf"
                },
                {
                    name: "Hosts",
                    path: "/etc/hosts"
                },
                {
                    name: "Simple Desktop Display Manager",
                    path: "/etc/tlp.conf"
                },
                {
                    name: "Domain Name Server",
                    path: "/etc/resolv.conf"
                },
                {
                    name: "Filesystem Table",
                    path: "/etc/fstab"
                }
            ]
        }
    }
    Button {
        id: editButton
        text: "Edit"
        visible: true
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: cflTable.bottom
        anchors.topMargin: 10
        onClicked: console.log("editButton pressed")
    }
}