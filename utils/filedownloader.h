#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>
#include <QUrl>

class ImageDownloader : public QObject
{
    Q_OBJECT

public:
    explicit ImageDownloader(QObject *parent = nullptr);

    void downloadImage(const QUrl &url);

signals:
    void imageDownloaded(const QByteArray &image);
    void downloadFailed(const QString &errorString);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager m_networkManager;
};
