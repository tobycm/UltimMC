#pragma once

#include <QObject>
#include "QObjectPtr.h"
#include <QDateTime>
#include <QSet>
#include <QProcess>
#include <QDebug>

#include "BaseAuthProvider.h"

class LocalAuthProvider : public BaseAuthProvider
{
    Q_OBJECT

public:
    QString id()
    {
        return "local";
    }

    QString displayName()
    {
        return "Local";
    }

    bool localAuth()
    {
        return true;
    }

    QString injectorEndpoint()
    {
        return ((QString)"http://localhost:%1").arg(m_authServer->port());
    };

    QString authEndpoint()
    {
        return ((QString) "http://localhost:%1/auth/").arg(m_authServer->port());
    };

    virtual bool useYggdrasil()
    {
        return true;
    }
};
