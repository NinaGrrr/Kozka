import QtQuick
import QtQuick.Controls
import Fiszki 1.0 // Importujemy naszą wtyczkę z C++!

Window {
    width: 400
    height: 300
    visible: true
    title: "Kózka"

    FlashcardManager {
        id: logika
    }

    Text {
        anchors.centerIn: parent
        font.pixelSize: 50
        text: logika.czyOdwrocona ? logika.aktualneTlumaczenie : logika.aktualneSlowo
    }

    Row {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 30
        spacing: 20

        Button {
            text: "Odwróć"
            width: 120
            height: 50
            onClicked: logika.odwrocFiszke()
        }

        Button {
            text: "Następna"
            width: 120
            height: 50
            onClicked: logika.nastepnaFiszka()
        }
    }
}