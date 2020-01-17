#include "downloadclient.h"

DownloadClient::DownloadClient()
{

}

DownloadClient::~DownloadClient()
{
    settings->sync();
    m_config->deleteLater();
    m_networkManager->deleteLater();
    m_reply->deleteLater();
}

QString DownloadClient::get(int type) const
{
    switch (type) {
    case PID:
        return "0004";
    case NAME:
        return "DownloadClient";
    case PTYPE:
        return "PLUGIN";
    case PSETTINGS:
        return corelibrary->getAppDir().absolutePath() + "/settings/settings.ini";
    case VERSION:
        return "VERSION_1.0.0";
    default:
        break;
    }
    return "";
}

int DownloadClient::onInit(int type)
{
    switch (type) {
    case FIRSTINIT:
    {
        /* plugin general */
        m_config = new DownloadClientConfig(this);
        corelibrary->subscribeEvent("START_DOWNLOAD", this);

        loadSettings();
        qInfo() << get(NAME) << "first init done!";
    }
        break;
    case EXTENDEDINIT:
    {
        /* networkaccessmanager operations */
        m_networkManager = new QNetworkAccessManager();
        connect(m_networkManager, &QNetworkAccessManager::finished, this, &DownloadClient::slotRequestFinished);

        qInfo() << get(NAME) << "extended init done!";
    }
        break;
    case EXIT:
        break;
    default:
        break;
    }
    return 0;
}

bool DownloadClient::onEvent(CustomEvent *customEvent)
{
    QString print;
    if (!customEvent->getCustomEvent(COMMAND).isEmpty())
    {
        QString command = customEvent->getCustomEvent(COMMAND);
        QString type = customEvent->getCustomEvent(TYPE);
        QString data = customEvent->getCustomEvent(DATA);
        if (command == "START_DOWNLOAD") {

            QByteArray byteMessage;
            byteMessage = customEvent->getCustomEvent(DATA).toUtf8().data();
            QString url = customEvent->getCustomEvent(TYPE);
            QString fileName = url.mid(url.lastIndexOf("/")+1);

            qInfo() << get(NAME) + " onEvent: " + "Command: " + command  + " Type: " + type + " Url: " + url;

            downloadPlaylistItem(fileName,url);
        }
    }
    return false;
}

void DownloadClient::loadSettings()
{
    settings = new QSettings(get(PSETTINGS),QSettings::IniFormat);

    settings->beginGroup(get(NAME));
    settings->endGroup();
    settings->sync();
    //qInfo() << get(NAME) + " Loaded settings: ";
}

void DownloadClient::saveSettings()
{
    settings->beginGroup(get(NAME));
    settings->sync();
    settings->endGroup();

    //qInfo() << get(NAME) + " Saved settings: ";
}

void DownloadClient::downloadPlaylistItem(QString fileName, QString url)
{
    QNetworkRequest req;
    QUrl downloadURL(url);
    if(!downloadURL.isValid()) {
        qDebug() << "Invalid URL in Downloader. FileName: " + fileName + " URL: " + url;
        emit downloadError(fileName);

        CustomEvent cEvent;
        cEvent.addCustomEvent(SENDER,"MANAGER");
        cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
        cEvent.addCustomEvent(TYPE,"ERROR");
        cEvent.addCustomEvent(IDENT,m_fileName);
        cEvent.addCustomEvent(DATA,"Invalid URL in Downloader. FileName: " + fileName + " URL: " + url);
        corelibrary->addEvent(&cEvent);
    }

    m_fileName = corelibrary->getAppDir().absolutePath() + "/content_downloaded/" + fileName;
    req.setUrl(downloadURL);

    m_reply = m_networkManager->get(req);
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

void DownloadClient::slotReadyRead()
{
    if(m_reply) {
        if(m_reply->error()==QNetworkReply::NoError) {
            if(!m_file.isOpen()) {
                m_file.setFileName(m_fileName);
                if(!m_file.open(QFile::WriteOnly | QFile::Append)) {
                    qDebug() << "Cant open file: " + m_fileName;
                    emit downloadError(m_fileName);
                    m_reply->abort();

                    CustomEvent cEvent;
                    cEvent.addCustomEvent(SENDER,"MANAGER");
                    cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
                    cEvent.addCustomEvent(TYPE,"ERROR");
                    cEvent.addCustomEvent(IDENT,m_fileName);
                    cEvent.addCustomEvent(DATA,"Cant open file: " + m_fileName);
                    corelibrary->addEvent(&cEvent);
                }
            }

            if(m_file.isOpen() && m_reply) {
                QByteArray array;
                while(m_reply->bytesAvailable()) {
                    array=m_reply->readAll();
                    m_file.write(array);
                }
            }

        }
        else {
            if(m_file.isOpen())
                m_file.close();
            qDebug() << "Error in network-communication. readyRead has occured an error";
            emit downloadError(m_fileName);

            CustomEvent cEvent;
            cEvent.addCustomEvent(SENDER,"MANAGER");
            cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
            cEvent.addCustomEvent(TYPE,"ERROR");
            cEvent.addCustomEvent(IDENT,m_fileName);
            cEvent.addCustomEvent(DATA,"Error in network-communication. readyRead has occured an error");
            corelibrary->addEvent(&cEvent);
        }
    }
    else {
        qDebug() << "currReply is null in readyread";
        emit downloadError(m_fileName);

        CustomEvent cEvent;
        cEvent.addCustomEvent(SENDER,"MANAGER");
        cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
        cEvent.addCustomEvent(TYPE,"ERROR");
        cEvent.addCustomEvent(IDENT,m_fileName);
        cEvent.addCustomEvent(DATA,"currReply is null in readyread");
        corelibrary->addEvent(&cEvent);
    }
    if(m_file.isOpen()) {
        m_file.close();
    }
}

void DownloadClient::slotRequestFinished(QNetworkReply *reply)
{
    if(reply) {
        if(reply->error()==QNetworkReply::NoError) {
            if(!m_file.isOpen()) {
                m_file.setFileName(m_fileName);
                if(!m_file.open(QFile::WriteOnly | QFile::Append)) {
                    qDebug() << "Cant open file: " + m_fileName;
                    reply->abort();

                    CustomEvent cEvent;
                    cEvent.addCustomEvent(SENDER,"MANAGER");
                    cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
                    cEvent.addCustomEvent(TYPE,"ERROR");
                    cEvent.addCustomEvent(IDENT,m_fileName);
                    cEvent.addCustomEvent(DATA,"Cant open file: " + m_fileName);
                    corelibrary->addEvent(&cEvent);
                }
            }

            if(m_file.isOpen() && reply) {
                QByteArray array;
                while(reply->bytesAvailable()) {
                    array=reply->readAll();
                    m_file.write(array);
                }
            }
        }
        else {
            if(m_file.isOpen())
                m_file.close();
            qDebug() << "Error in network-communication. ReadyRead has occured an error";

            CustomEvent cEvent;
            cEvent.addCustomEvent(SENDER,"MANAGER");
            cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
            cEvent.addCustomEvent(TYPE,"ERROR");
            cEvent.addCustomEvent(IDENT,m_fileName);
            cEvent.addCustomEvent(DATA,"Error in network-communication. ReadyRead has occured an error");
            corelibrary->addEvent(&cEvent);
        }
    }
    else {
        qDebug() << "currReply is null in readyread";
        CustomEvent cEvent;
        cEvent.addCustomEvent(SENDER,"MANAGER");
        cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
        cEvent.addCustomEvent(TYPE,"ERROR");
        cEvent.addCustomEvent(IDENT,m_fileName);
        cEvent.addCustomEvent(DATA,"currReply is null in readyread");
        corelibrary->addEvent(&cEvent);
    }

    if(m_file.isOpen()) {
        m_file.close();
    }
    qDebug() << "Reply Finished -> Content Downloaded successfully!!! -> " + m_fileName;

    reply->deleteLater();
    m_reply = nullptr;

    CustomEvent cEvent;
    cEvent.addCustomEvent(SENDER,"MANAGER");
    cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
    cEvent.addCustomEvent(TYPE,"SUCCESS");
    cEvent.addCustomEvent(IDENT,m_fileName);
    cEvent.addCustomEvent(DATA,"");
    corelibrary->addEvent(&cEvent);
}

void DownloadClient::slotError(QNetworkReply::NetworkError err)
{
    Q_UNUSED(err)
    if(m_reply) {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    emit downloadError(m_fileName);

    CustomEvent cEvent;
    cEvent.addCustomEvent(SENDER,"MANAGER");
    cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
    cEvent.addCustomEvent(TYPE,"ERROR");
    cEvent.addCustomEvent(IDENT,m_fileName);
    cEvent.addCustomEvent(DATA,"");
    corelibrary->addEvent(&cEvent);
}

void DownloadClient::slotSslErrors(QList<QSslError> sslErr)
{
    Q_UNUSED(sslErr)
    if(m_reply) {
        m_reply->deleteLater();
        m_reply = nullptr;
    }
    emit downloadError(m_fileName);

    CustomEvent cEvent;
    cEvent.addCustomEvent(SENDER,"MANAGER");
    cEvent.addCustomEvent(COMMAND,"FINISHED_DOWNLOAD");
    cEvent.addCustomEvent(TYPE,"ERROR");
    cEvent.addCustomEvent(IDENT,m_fileName);
    cEvent.addCustomEvent(DATA,"");
    corelibrary->addEvent(&cEvent);
}

