#pragma once
#include <QObject>

#include "QObjectPtr.h"
#include "minecraft/auth/AuthStep.h"

#include <katabasis/DeviceFlow.h>

class LocalStep : public AuthStep {
    Q_OBJECT
   public:
    explicit LocalStep(AccountData* data);
    virtual ~LocalStep() noexcept;

    void perform() override;
    void rehydrate() override;

    QString describe() override;
};
