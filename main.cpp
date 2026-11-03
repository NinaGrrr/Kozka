#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include "flashcardmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<FlashcardManager>("Fiszki", 1, 0, "FlashcardManager");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Kozka", "Main");

    return QGuiApplication::exec();
}
