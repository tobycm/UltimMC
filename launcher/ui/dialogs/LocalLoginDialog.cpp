#include "LocalLoginDialog.h"
#include "ui_LocalLoginDialog.h"

#include "minecraft/auth/AuthProviders.h"
#include "minecraft/auth/AccountTask.h"

#include <QtWidgets/QPushButton>

LocalLoginDialog::LocalLoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::LocalLoginDialog)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

LocalLoginDialog::~LocalLoginDialog()
{
    delete ui;
}

// Stage 1: User interaction
void LocalLoginDialog::accept()
{
    setUserInputsEnabled(false);
    ui->progressBar->setVisible(true);

    m_account = MinecraftAccount::createLocal(ui->userTextBox->text());
    m_account->setProvider(AuthProviders::lookup("local"));

    // Setup the login task and start it
    m_loginTask = m_account->loginLocal();
    connect(m_loginTask.get(), &Task::failed, this, &LocalLoginDialog::onTaskFailed);
    connect(m_loginTask.get(), &Task::succeeded, this, &LocalLoginDialog::onTaskSucceeded);
    connect(m_loginTask.get(), &Task::status, this, &LocalLoginDialog::onTaskStatus);
    connect(m_loginTask.get(), &Task::progress, this, &LocalLoginDialog::onTaskProgress);
    if (!m_loginTask)
    {
        onTaskSucceeded();
    } else {
        m_loginTask->start();
    }
}

void LocalLoginDialog::setUserInputsEnabled(bool enable)
{
    ui->userTextBox->setEnabled(enable);
    ui->buttonBox->setEnabled(enable);
}

void LocalLoginDialog::on_userTextBox_textEdited(const QString &newText)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)
        ->setEnabled(!newText.isEmpty());
}

void LocalLoginDialog::onTaskFailed(const QString &reason)
{
    // Set message
    auto lines = reason.split('\n');
    QString processed;
    for(auto line: lines) {
        if(line.size()) {
            processed += "<font color='red'>" + line + "</font><br />";
        }
        else {
            processed += "<br />";
        }
    }
    ui->label->setText(processed);

    // Re-enable user-interaction
    setUserInputsEnabled(true);
    ui->progressBar->setVisible(false);
}

void LocalLoginDialog::onTaskSucceeded()
{
    QDialog::accept();
}

void LocalLoginDialog::onTaskStatus(const QString &status)
{
    ui->label->setText(status);
}

void LocalLoginDialog::onTaskProgress(qint64 current, qint64 total)
{
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

// Public interface
MinecraftAccountPtr LocalLoginDialog::newAccount(QWidget *parent, QString msg)
{
    LocalLoginDialog dlg(parent);
    dlg.ui->label->setText(msg);
    if (dlg.exec() == QDialog::Accepted)
    {
        return dlg.m_account;
    }
    return 0;
}
