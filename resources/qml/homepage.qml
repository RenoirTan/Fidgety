import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
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
    Button {
        text: "Edit"
        anchors.horizontalCenter: parent.horizontalCenter
    }
}