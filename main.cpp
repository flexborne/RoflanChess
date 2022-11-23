#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>

#include "game.h"
#include "boardModel.h"
#include "defines.h"

#include "spdlog/spdlog.h"

int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::debug);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qmlRegisterUncreatableMetaObject(
      Side::staticMetaObject,
      "enum_side",
      1, 0,
      "Side",
      "Error: only enums"
    );

    qmlRegisterUncreatableMetaObject(
      GameStatus::staticMetaObject,
      "enum_gameStatus",
      1, 0,
      "GameStatus",
      "Error: only enums"
    );

    QQmlApplicationEngine engine;

    Game game;

    engine.rootContext()->setContextProperty(QStringLiteral("boardModel"), game.getBoardModel().get());

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
