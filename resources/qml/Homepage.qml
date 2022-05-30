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
            title: "Configuration Files"
            MenuItem {
                text: "/etc/default/grub"
                onTriggered: console.log("User has selected '/etc/default/grub'")
            }
            MenuItem {
                text: "/etc/tlp.conf"
                onTriggered: console.log("User has selected '/etc/tlp.conf'")
            }
        }
    }
    ListView {
        id: configFileList
        width: 300
        height: 200
        visible: true
        model: ConfigFilesModel {}
        delegate: Text {
            text: name + ": " + path
            color: "white"
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