import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    id: homepage
    visible: true
    width: 640
    height: 480
    title: "Fidgety"
    ListView {
        width: 300, 440
        model: ConfigFilesModel {}
        delegate: Text {
            text: name + ": " + path
            color: "white"
        }
    }
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
    Button {
        text: "Edit"
        anchors.horizontalCenter: parent.horizontalCenter
    }
}