#ifndef FLASHCARDMANAGER_H
#define FLASHCARDMANAGER_H

#include <QObject>

class FlashcardManager : public QObject
{
    Q_OBJECT
public:
    explicit FlashcardManager(QObject *parent = nullptr);

signals:
};

#endif // FLASHCARDMANAGER_H
