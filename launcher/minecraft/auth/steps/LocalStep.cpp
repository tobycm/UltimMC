#include "LocalStep.h"

#include "Application.h"

// This step does nothing but just log that we created a local account.

LocalStep::LocalStep(AccountData* data) : AuthStep(data) {}
LocalStep::~LocalStep() noexcept = default;

QString LocalStep::describe()
{
    return tr("Creating local account.");
}

void LocalStep::rehydrate()
{
    // NOOP
}

void LocalStep::perform()
{
    emit finished(AccountTaskState::STATE_WORKING, tr("Created local account."));
}
