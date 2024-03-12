#include "Elyby.h"

#include "minecraft/auth/steps/YggdrasilStep.h"
#include "minecraft/auth/steps/ElybyProfileStep.h"

ElybyRefresh::ElybyRefresh(
    AccountData *data,
    QObject *parent
) : AuthFlow(data, parent) {
    m_steps.append(new YggdrasilStep(m_data, QString()));
    m_steps.append(new ElybyProfileStep(m_data));
}

ElybyLogin::ElybyLogin(
    AccountData *data,
    QString password,
    QObject *parent
): AuthFlow(data, parent), m_password(password) {
    m_steps.append(new YggdrasilStep(m_data, m_password));
    m_steps.append(new ElybyProfileStep(m_data));
}
