#pragma once
#include "AuthFlow.h"

class LocalRefresh : public AuthFlow
{
    Q_OBJECT
public:
    explicit LocalRefresh(
        AccountData *data,
        QObject *parent = 0
    );
};

class LocalLogin : public AuthFlow
{
    Q_OBJECT
public:
    explicit LocalLogin(
        AccountData *data,
        QObject *parent = 0
    );
};
