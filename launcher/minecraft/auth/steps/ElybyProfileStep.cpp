#include "ElybyProfileStep.h"

#include <QNetworkRequest>

#include "minecraft/auth/AuthRequest.h"
#include "minecraft/auth/Parsers.h"

ElybyProfileStep::ElybyProfileStep(AccountData* data) : AuthStep(data) {

}

ElybyProfileStep::~ElybyProfileStep() noexcept = default;

QString ElybyProfileStep::describe() {
    return tr("Fetching the Ely.by profile.");
}


void ElybyProfileStep::perform() {
    auto url = QUrl(QString("https://authserver.ely.by/api/users/profiles/minecraft/%1").arg(m_data->userName()).toUtf8());
    QNetworkRequest request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    AuthRequest *requestor = new AuthRequest(this);
    connect(requestor, &AuthRequest::finished, this, &ElybyProfileStep::onRequestDone);
    requestor->get(request);
}

void ElybyProfileStep::rehydrate() {
    // NOOP, for now. We only save bools and there's nothing to check.
}

void ElybyProfileStep::onRequestDone(
    QNetworkReply::NetworkError error,
    QByteArray data,
    QList<QNetworkReply::RawHeaderPair> headers
) {
    auto requestor = qobject_cast<AuthRequest *>(QObject::sender());
    requestor->deleteLater();

#ifndef NDEBUG
    qDebug() << data;
#endif
    if (error != QNetworkReply::NoError) {
        qWarning() << "Error getting profile:";
        qWarning() << " HTTP Status:        " << requestor->httpStatus_;
        qWarning() << " Internal error no.: " << error;
        qWarning() << " Error string:       " << requestor->errorString_;

        qWarning() << " Response:";
        qWarning() << QString::fromUtf8(data);

        emit finished(
            AccountTaskState::STATE_FAILED_SOFT,
            tr("Minecraft Java profile acquisition failed.")
        );
        return;
    }
    if(!Parsers::parseMinecraftProfile(data, m_data->minecraftProfile)) {
        m_data->minecraftProfile = MinecraftProfile();
        emit finished(
            AccountTaskState::STATE_FAILED_SOFT,
            tr("Minecraft Java profile response could not be parsed")
        );
        return;
    }

    emit finished(
        AccountTaskState::STATE_WORKING,
        tr("Minecraft Java profile acquisition succeeded.")
    );
}
