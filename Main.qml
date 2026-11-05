import QtQuick
import QtQuick.Controls
import QtQuick.Window
import Fiszki 1.0 // Importujemy naszą wtyczkę z C++!

Window {
    width: 400
    height: 300
    visible: true
    title: "Kózka"
    color: "#F4F4F9"

    FlashcardManager {
        id: logika
    }

    Flipable {
            id: karta
            width: parent.width * 0.85
            height: parent.height * 0.55
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -40
            property bool obrocona: logika.czyOdwrocona
            front: Rectangle {
                anchors.fill: parent
                color: "white"
                radius: 20
                border.color: "#E5E5EA"
                border.width: 2
                Text {
                    anchors.centerIn: parent
                    font.family: "Segoe UI"
                    font.pixelSize: 42
                    font.weight: Font.DemiBold
                    color: "#2C3E50"
                    text: logika.koniecNauki ? "Gratulacje! xD\nKoniec na dziś." : logika.aktualneSlowo
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            back: Rectangle {
                anchors.fill: parent
                color: "white"
                radius: 20
                border.color: "#E5E5EA"
                border.width: 2
                Text {
                    anchors.centerIn: parent
                    font.family: "Segoe UI"
                    font.pixelSize: 42
                    font.weight: Font.DemiBold
                    color: "#2C3E50"
                    text: logika.aktualneTlumaczenie
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            transform: Rotation {
                id: osObrotu
                origin.x: karta.width / 2
                origin.y: karta.height / 2
                axis.x: 0; axis.y: 1; axis.z: 0
                angle: karta.obrocona ? 180 : 0

                Behavior on angle {
                    NumberAnimation {
                        duration: 400
                        easing.type: Easing.InOutQuad
                    }
                }
            }
        }

        Row {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 40
            spacing: 20
            visible: !logika.koniecNauki

            Button {
                width: 140
                height: 54
                contentItem: Text {
                    text: "Odwróć"
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                background: Rectangle {
                    color: parent.pressed ? "#34495E" : "#2C3E50"
                }

                onClicked: logika.odwrocFiszke()
            }
            Button {
                width: 140
                height: 54
                contentItem: Text {
                    text: "Następna"
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: parent.pressed ? "#27AE60" : "#2ECC71"
                    radius: 27
                }
                onClicked: logika.nastepnaFiszka()
            }
        }
    }