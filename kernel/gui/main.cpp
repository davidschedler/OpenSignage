#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "engine.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    for (int i = 0 ; i < argc ; i++)
    {
        if (argv[i] == QString("--debug"))
        {
            //#define DEBUG
        }
    }

    Engine myEngine;
    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("myEngine", &myEngine);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
