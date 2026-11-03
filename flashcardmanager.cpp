#include "flashcardmanager.h"
#include <QDebug>
#include <QRandomGenerator>

FlashcardManager::FlashcardManager(QObject *parent)
    : QObject(parent), m_aktualnyIndeks(0), m_czyOdwrocona(false), m_koniecNauki(false)
{
    wczytajZPliku("C:/Users/karok/Desktop/Pryff/Projekty_wy/Kozka/fiszki.json");
}

// --- GETTERY (QML używa ich, gdy odbierze sygnał NOTIFY) ---
QString FlashcardManager::pobierzSlowo() const { return m_aktualneSlowo; }
QString FlashcardManager::pobierzTlumaczenie() const { return m_aktualneTlumaczenie; }
bool FlashcardManager::pobierzCzyOdwrocona() const { return m_czyOdwrocona; }
bool FlashcardManager::pobierzKoniecNauki() const { return m_koniecNauki; }
// --- SLOTY (Reakcje na fizyczne kliknięcia w QML) ---
void FlashcardManager::odwrocFiszke()
{
    m_czyOdwrocona = !m_czyOdwrocona;
    emit odwrocenieZmienione();
}

void FlashcardManager::nastepnaFiszka()
{
    if (m_nieprzejrzaneIndeksy.empty()) {
        m_koniecNauki = true;
        emit koniecNaukiZmieniony();
        return;
    }
    int wylosowanaPozycja = QRandomGenerator::global()->bounded(static_cast<int>(m_nieprzejrzaneIndeksy.size()));
    m_aktualnyIndeks = m_nieprzejrzaneIndeksy[wylosowanaPozycja];
    m_nieprzejrzaneIndeksy[wylosowanaPozycja] = m_nieprzejrzaneIndeksy.back();
    m_nieprzejrzaneIndeksy.pop_back();
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
    QFile plik(sciezka);
    if (!plik.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "BŁĄD: Nie udało się otworzyć pliku:" << sciezka;
        return;
    }
    QByteArray dane = plik.readAll();
    plik.close();
    QJsonDocument doc = QJsonDocument::fromJson(dane);
    if (!doc.isArray()) {
        qDebug() << "BŁĄD: Plik JSON nie zawiera tablicy [] na samej górze!";
        return;
    }
    m_talia.clear();
    QJsonArray tablica = doc.array();
    for (const QJsonValue &wartosc : tablica) {
        QJsonObject obiekt = wartosc.toObject();

        Fiszka nowaFiszka;
        nowaFiszka.id = obiekt["id"].toInt();
        nowaFiszka.slowo = obiekt["slowo"].toString();
        nowaFiszka.tlumaczenie = obiekt["tlumaczenie"].toString();

        m_talia.push_back(nowaFiszka);
    }

    if (!m_talia.empty()) {
        m_koniecNauki = false;
        m_nieprzejrzaneIndeksy.clear();
        for (int i = 0; i < m_talia.size(); ++i) {
            m_nieprzejrzaneIndeksy.push_back(i);
        }
        emit koniecNaukiZmieniony();
        nastepnaFiszka(); // Od razu losujemy pierwszą fiszkę z worka
        qDebug() << "SUKCES: Załadowano fiszek:" << m_talia.size();
    }
}