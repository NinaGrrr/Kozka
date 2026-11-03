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
    Q_PROPERTY(bool koniecNauki READ pobierzKoniecNauki NOTIFY koniecNaukiZmieniony)
public:
    explicit FlashcardManager(QObject *parent = nullptr);
    QString pobierzSlowo() const;
    QString pobierzTlumaczenie() const;
    bool pobierzCzyOdwrocona() const;
    bool pobierzKoniecNauki() const;

signals:
    void fiszkaZmieniona();
    void odwrocenieZmienione();
    void koniecNaukiZmieniony();

public slots:
    void nastepnaFiszka();
    void odwrocFiszke();
    void zaladujTestoweDane();
    void wczytajZPliku(const QString &sciezka);
private:
    std::vector<Fiszka> m_talia;
    int m_aktualnyIndeks;
    QString m_aktualneSlowo;
    QString m_aktualneTlumaczenie;
    bool m_czyOdwrocona;
    void aktualizujStanDlaEkranu();
    bool m_koniecNauki;
    std::vector<int> m_nieprzejrzaneIndeksy;
};

#endif // FLASHCARDMANAGER_H
