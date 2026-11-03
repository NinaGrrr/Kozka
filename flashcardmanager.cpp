#include "flashcardmanager.h"
#include <QDebug>
#include <QRandomGenerator>

FlashcardManager::FlashcardManager(QObject *parent)
    : QObject(parent), m_aktualnyIndeks(0), m_czyOdwrocona(false)
{
    wczytajZPliku("C:/Users/karok/Desktop/Pryff/Projekty_wy/Kozka/fiszki.json");
}

// --- GETTERY (QML używa ich, gdy odbierze sygnał NOTIFY) ---
QString FlashcardManager::pobierzSlowo() const { return m_aktualneSlowo; }
QString FlashcardManager::pobierzTlumaczenie() const { return m_aktualneTlumaczenie; }
bool FlashcardManager::pobierzCzyOdwrocona() const { return m_czyOdwrocona; }

// --- SLOTY (Reakcje na fizyczne kliknięcia w QML) ---
void FlashcardManager::odwrocFiszke()
{
    m_czyOdwrocona = !m_czyOdwrocona;
    emit odwrocenieZmienione();
}

void FlashcardManager::nastepnaFiszka()
{
    if (m_talia.empty()) return;

    if (m_talia.size() == 1) {
        aktualizujStanDlaEkranu();
        return;
    }
    int staryIndeks = m_aktualnyIndeks;
    int nowyIndeks = staryIndeks;
    while (nowyIndeks == staryIndeks) {
        nowyIndeks = QRandomGenerator::global()->bounded(static_cast<int>(m_talia.size()));
    }

    m_aktualnyIndeks = nowyIndeks;
    aktualizujStanDlaEkranu();
}

// --- WEWNĘTRZNA LOGIKA ---
void FlashcardManager::aktualizujStanDlaEkranu()
{
    if (m_talia.empty()) return;

    Fiszka obecnaFiszka = m_talia[m_aktualnyIndeks];
    m_aktualneSlowo = obecnaFiszka.slowo;
    m_aktualneTlumaczenie = obecnaFiszka.tlumaczenie;

    m_czyOdwrocona = false;
    emit fiszkaZmieniona();
    emit odwrocenieZmienione();
}

void FlashcardManager::zaladujTestoweDane()
{
    m_talia.push_back({1, "회사", "Firma"});
    m_talia.push_back({2, "회의", "Spotkanie"});
    m_talia.push_back({3, "개발자", "Programista"});

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