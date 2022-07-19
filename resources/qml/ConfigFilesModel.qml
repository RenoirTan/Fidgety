import QtQuick 2.12
import Qt.labs.qmlmodels 1.0

TableModel {
    id: cflModel
    TableModelColumn { display: "name" }
    TableModelColumn { display: "path" }
    rows: [
        {
            name: "__fidgety_phantom_row__",
            path: "__fidgety_phantom_row__"
        },
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
        },
        {
            name: "SSH Daemon",
            path: "/etc/ssh/sshd_config"
        },
        {
            name: "Libvirt QEMU Driver",
            path: "/etc/libvirt/qemu.conf"
        },
        {
            name: "Lynx Browser System-wide Configuration",
            path: "/etc/lynx.cfg"
        }
    ]
}
