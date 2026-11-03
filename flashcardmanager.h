#ifndef FLASHCARDMANAGER_H
#define FLASHCARDMANAGER_H

#include <QObject>
#include <QString>
#include <vector>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

struct Fiszka {
    int id;
    QString slowo;
    QString tlumaczenie;
};

class FlashcardManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString aktualneSlowo READ pobierzSlowo NOTIFY fiszkaZmieniona)
    Q_PROPERTY(QString aktualneTlumaczenie READ pobierzTlumaczenie NOTIFY fiszkaZmieniona)
    Q_PROPERTY(bool czyOdwrocona READ pobierzCzyOdwrocona NOTIFY odwrocenieZmienione)
public:
    explicit FlashcardManager(QObject *parent = nullptr);
    QString pobierzSlowo() const;
    QString pobierzTlumaczenie() const;
    bool pobierzCzyOdwrocona() const;

signals:
    void fiszkaZmieniona();
    void odwrocenieZmienione();

public slots:
    void nastepnaFiszka();
    void odwrocFiszke();

    // Metoda tymczasowa - użyjemy jej, dopóki nie napiszemy loadera JSON
    void zaladujTestoweDane();
    void wczytajZPliku(const QString &sciezka);
private:
    // Pamięć masowa i stan układu
    std::vector<Fiszka> m_talia;
    int m_aktualnyIndeks;

    // Zmienne buforowe, z których bezpośrednio czyta interfejs QML
    QString m_aktualneSlowo;
    QString m_aktualneTlumaczenie;
    bool m_czyOdwrocona;

    // Prywatna metoda pomocnicza do ułożenia danych
    void aktualizujStanDlaEkranu();
};

#endif // FLASHCARDMANAGER_H
