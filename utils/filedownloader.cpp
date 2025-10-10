#include "filedownloader.h"
#include <QBuffer>
#include <iostream>
ImageDownloader::ImageDownloader(QObject *parent)
    : QObject(parent)
{
    connect(&m_networkManager, &QNetworkAccessManager::finished,
            this, &ImageDownloader::onReplyFinished);
}

void ImageDownloader::downloadImage(const QUrl &url)
{
    std::cout << "start downloading" << std::endl;
    QNetworkRequest request(url);
    m_networkManager.get(request);
}

void ImageDownloader::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit downloadFailed(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray imageData = reply->readAll();
    QImage image;
    std::cout << "finished downloading " << std::endl;
    if (!image.loadFromData(imageData)) {
        emit downloadFailed("Failed to decode image data");
    } else {
        emit imageDownloaded(imageData);
    }

    reply->deleteLater();
}
