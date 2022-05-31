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
        // alternatingRowColors: true
        //anchors.horizontalCenter: parent.horizontalCenter
        //anchors.topMargin: 25
        anchors.fill: parent
        anchors.horizontalCenter: parent.horizontalCenter
        boundsBehavior: Flickable.StopAtBounds
        model: TableModel {
            id: configFileListModel
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
        delegate: DelegateChooser {
            DelegateChoice {
                delegate: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 50
                    Text { text: display }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            let index = configFileListModel.index(row, column);
                            console.log("Selected " + index);
                            console.log("Row: ");
                            for (let i = 0; i < configFileListModel.columnCount; i++) {
                                let colindex = configFileListModel.index(row, i);
                                let data = configFileListModel.data(colindex, "");
                                console.log(data);
                            }
                        }
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
        anchors.top: configFileList.bottom
        anchors.topMargin: 10
        onClicked: console.log("editButton pressed")
    }
}