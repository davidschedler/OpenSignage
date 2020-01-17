#include <QCoreApplication>
#include <QFile>
#include <QProcess>

#include "corelibrary.h"

/**
 * @brief globalDebug Leite alle globalen Debug-Nachrichten um
 * @param type Debug-Type
 * @param context Debug-Context
 * @param msg Debug-Message
 */
void globalDebug(QtMsgType type, const QMessageLogContext &context, const QString &msg);

/**
 * @brief setupDirectories Erstelle Programmorder inkl. aller Programmunterordner
 * @param dir Ziel des Prorammordners
 * @return Absoluter Programmorder-Pfad
 */
QString setupDirectories(QDir dir);

int main(int argc, char *argv[])
{
    qInstallMessageHandler(globalDebug);
    qSetMessagePattern("[%{type}] %{appname} (%{file}:%{line}) - %{message}");

    QCoreApplication a(argc, argv);
//    QString port;
//    int loglevel = 1;
//    for (int i = 0 ; i < argc ; i++)
//    {
//        QString option = QString(argv[i]);

//        if (option.startsWith("-p")) {
//            if (option.length() == 2) {
//                port = QString(argv[i+1]).simplified().toInt();
//                i++;
//            }
//            else {
//                port = option.mid(2,option.length() - 2).simplified().toInt();
//            }
//        }
//        if (option.startsWith("-d")) {
//            if (option.length() == 2) {
//                loglevel = QString(argv[i+1]).simplified().toInt();
//                i++;
//            }
//            else {
//                loglevel = option.mid(2,option.length() - 2).simplified().toInt();
//            }
//        }
//    }
//    if (loglevel == 1) {
//#define DEBUG1
//    }
//    if (loglevel == 2) {
//#define DEBUG2
//    }


    QDir appDir;
    CoreLibrary core(setupDirectories(appDir.homePath()));

    QString pluginLocation = QDir::currentPath() + "/../../plugins/_plugins/cms/";
    core.initPlugins(pluginLocation);

    return a.exec();
}

void globalDebug(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

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
    QFile file(dir.homePath() + "/OpenSignage/debug/out_cms.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << txt << "\n";
    file.close();
}

QString setupDirectories(QDir dir)
{
    if (!dir.exists("OpenSignage"))
        dir.mkdir("OpenSignage");
    dir.cd("OpenSignage");

    if (!dir.exists("content_uploaded"))
        dir.mkdir("content_uploaded");
    if (!dir.exists("settings"))
        dir.mkdir("settings");
    if (!dir.exists("debug"))
        dir.mkdir("debug");
    if (!dir.exists("database"))
        dir.mkdir("database");

    return dir.absolutePath();
}
