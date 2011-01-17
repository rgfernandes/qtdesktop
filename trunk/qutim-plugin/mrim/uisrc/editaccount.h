#ifndef EDITACCOUNT_H
#define EDITACCOUNT_H

#include <QtGui/QWidget>
#include <QAbstractButton>

namespace Ui {
    class EditAccount;
}

class MRIMClient;
class LoginForm;
class SettingsWidget;

class EditAccount : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(EditAccount)
public:
    explicit EditAccount(MRIMClient* aClient, QWidget *parent = 0);    
    virtual ~EditAccount();

protected:
    virtual void changeEvent(QEvent *e);
    void SaveSettings();
private:
    Ui::EditAccount *m_ui;
    MRIMClient* m_client;
    LoginForm* m_loginWidget;
    SettingsWidget* m_settingsWidget;

private slots:
    void on_buttonBox_clicked(QAbstractButton* button);
    void on_useProfileCheckBox_clicked();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // EDITACCOUNT_H
