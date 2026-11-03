#include "flashcardmanager.h"
#include <QDebug>

FlashcardManager::FlashcardManager(QObject *parent)
    : QObject(parent), m_aktualnyIndeks(0), m_czyOdwrocona(false)
{
    // Na starcie programu ładujemy wektor danymi
    wczytajZPliku("C:/Users/karok/Desktop/Pryff/Projekty_wy/Kozka/fiszki.json");
}

// --- GETTERY (QML używa ich, gdy odbierze sygnał NOTIFY) ---
QString FlashcardManager::pobierzSlowo() const { return m_aktualneSlowo; }
QString FlashcardManager::pobierzTlumaczenie() const { return m_aktualneTlumaczenie; }
bool FlashcardManager::pobierzCzyOdwrocona() const { return m_czyOdwrocona; }

// --- SLOTY (Reakcje na fizyczne kliknięcia w QML) ---
void FlashcardManager::odwrocFiszke()
{
    // Odwracamy stan logiczny (działa jak bramka NOT)
    m_czyOdwrocona = !m_czyOdwrocona;

    // Wysyłamy sygnał na ekran, żeby QML pokazał lub ukrył polski tekst
    emit odwrocenieZmienione();
}

void FlashcardManager::nastepnaFiszka()
{
    if (m_talia.empty()) return; // Zabezpieczenie przed próbą odczytu z pustej pamięci

    // Przesuwamy wskaźnik na kolejny element w wektorze
    m_aktualnyIndeks++;

    // Zabezpieczenie przed wyjściem poza tablicę - zapętlamy talię
    if (m_aktualnyIndeks >= m_talia.size()) {
        m_aktualnyIndeks = 0;
    }

    aktualizujStanDlaEkranu();
}

// --- WEWNĘTRZNA LOGIKA ---
void FlashcardManager::aktualizujStanDlaEkranu()
{
    if (m_talia.empty()) return;

    // Wyciągamy strukturę z wektora i kopiujemy jej dane do buforów dla ekranu
    Fiszka obecnaFiszka = m_talia[m_aktualnyIndeks];
    m_aktualneSlowo = obecnaFiszka.slowo;
    m_aktualneTlumaczenie = obecnaFiszka.tlumaczenie;

    // Przy zmianie słówka zawsze upewniamy się, że odpowiedź jest ukryta
    m_czyOdwrocona = false;

    // Odpalamy przerwania! Ekran natychmiast odczyta nowe wartości przez Gettery.
    emit fiszkaZmieniona();
    emit odwrocenieZmienione();
}

void FlashcardManager::zaladujTestoweDane()
{
    // Symulacja, jakbyśmy wczytali plik JSON - wrzucamy struktury do wektora
    m_talia.push_back({1, "회사", "Firma"});
    m_talia.push_back({2, "회의", "Spotkanie"});
    m_talia.push_back({3, "개발자", "Programista"});

    // Ustawiamy pierwszy element na ekran
    aktualizujStanDlaEkranu();
}

void FlashcardManager::wczytajZPliku(const QString &sciezka)
{
    // 1. Otwieramy plik w trybie tylko do odczytu
    QFile plik(sciezka);
    if (!plik.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "BŁĄD: Nie udało się otworzyć pliku:" << sciezka;
        return;
    }

    // 2. Wczytujemy całą zawartość i zamykamy plik
    QByteArray dane = plik.readAll();
    plik.close();

    // 3. Tłumaczymy surowy tekst na obiekt JSON
    QJsonDocument doc = QJsonDocument::fromJson(dane);
    if (!doc.isArray()) {
        qDebug() << "BŁĄD: Plik JSON nie zawiera tablicy [] na samej górze!";
        return;
    }

    // 4. Czyścimy starą talię i wrzucamy nowe słówka
    m_talia.clear();
    QJsonArray tablica = doc.array();

    for (const QJsonValue &wartosc : tablica) {
        QJsonObject obiekt = wartosc.toObject();

        Fiszka nowaFiszka;
        // Qt automatycznie rzutuje dane na odpowiednie typy (int, string)
        nowaFiszka.id = obiekt["id"].toInt();
        nowaFiszka.slowo = obiekt["slowo"].toString();
        nowaFiszka.tlumaczenie = obiekt["tlumaczenie"].toString();

        m_talia.push_back(nowaFiszka);
    }

    // 5. Jeśli udało się coś załadować, ustawiamy wskaźnik na początek i odświeżamy ekran
    if (!m_talia.empty()) {
        m_aktualnyIndeks = 0;
        aktualizujStanDlaEkranu();
        qDebug() << "SUKCES: Załadowano fiszek:" << m_talia.size();
    }
}