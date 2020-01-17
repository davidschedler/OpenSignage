#define DEBUG

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QWindow>
#include <QFile>

#include "corelibrary.h"
#include "manager.h"

void globalDebug(QtMsgType type, const QMessageLogContext &context, const QString &msg);
QString setupDirectories(QDir dir);

int main(int argc, char *argv[])
{
    qInstallMessageHandler(globalDebug);
    qSetMessagePattern("[%{type}] %{appname} (%{file}:%{line}) - %{message}");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    for (int i = 0 ; i < argc ; i++)
    {
        if (argv[i] == QString("--debug"))
        {
            //#define DEBUG
        }
    }

    QDir appDir;
    CoreLibrary core(setupDirectories(appDir.homePath()));

    QString pluginLocation = QDir::currentPath() + "/../../plugins/_plugins/raspberry/";
    core.initPlugins(pluginLocation);

    Manager myManager;
    myManager.corelibrary = &core;
    myManager.onInit(FIRSTINIT);
    myManager.onInit(EXTENDEDINIT);
    QQmlContext *ctx = engine.rootContext();
    ctx->setContextProperty("myManager", &myManager);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    //set window id to put vlc inside mainwindow
    QObject* m_rootObject = engine.rootObjects().first();
    if(m_rootObject) {
        QWindow *window = qobject_cast<QWindow *>(m_rootObject);
        if(window) {
            WId wid = window->winId();
            QString widString = QString::number(wid);
            CustomEvent cEvent;
            cEvent.addCustomEvent(SENDER,"MANAGER");
            cEvent.addCustomEvent(COMMAND,"PLAYER");
            cEvent.addCustomEvent(TYPE,"WINID");
            cEvent.addCustomEvent(IDENT,widString);
            cEvent.addCustomEvent(DATA,"");
            core.addEvent(&cEvent);
        }
    }


    return app.exec();
}

void globalDebug(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
#ifdef DEBUG
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;
    case QtInfoMsg: // Default info messages should be shown inside console
        txt = QString("Info: %1").arg(msg);
        QTextStream(stdout) << QString("Info: %1 \n").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        break;
    }

    QDir dir;
    QFile file(dir.homePath() + "/OpenSignage/debug/out_raspberry.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << txt << "\n";
    file.close();
#endif
}

QString setupDirectories(QDir dir)
{
    if (!dir.exists("OpenSignage"))
        dir.mkdir("OpenSignage");
    dir.cd("OpenSignage");

    if (!dir.exists("content_downloaded"))
        dir.mkdir("content_downloaded");
    if (!dir.exists("settings"))
        dir.mkdir("settings");
    if (!dir.exists("debug"))
        dir.mkdir("debug");
    if (!dir.exists("playlist"))
        dir.mkdir("playlist");

    return dir.absolutePath();
}
