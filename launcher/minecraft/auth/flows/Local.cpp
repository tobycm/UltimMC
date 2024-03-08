#include "Local.h"

#include "minecraft/auth/steps/LocalStep.h"

LocalRefresh::LocalRefresh(
    AccountData *data,
    QObject *parent
) : AuthFlow(data, parent) {
    m_steps.append(new LocalStep(m_data));
}

LocalLogin::LocalLogin(
    AccountData *data,
    QObject *parent
): AuthFlow(data, parent) {
    m_steps.append(new LocalStep(m_data));
}
