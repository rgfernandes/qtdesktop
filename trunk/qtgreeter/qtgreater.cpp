#include <QtGui/QApplication>

#include <QDebug>
#include <QFile>
#include <QListWidgetItem>
#include <QLabel>

#include <lightdm/ldmuser.h>
#include <lightdm/ldmsession.h>

#include "qtgreater.h"

Widget::Widget(LdmGreeter* greeter, QWidget *parent) :
    QWidget(greeter),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_greeter = greeter;

    ui->hostnameLabel->setText(m_greeter->hostname());

    QList<LdmUser> users = m_greeter->users();
    foreach(LdmUser user, users)
    {
        QListWidgetItem* item = new QListWidgetItem(user.displayName(), ui->userList);
        item->setData(Qt::UserRole, user.name());
        if(user.image().isEmpty())
        {
            item->setIcon(QIcon::fromTheme("user-identity"));
        }
        else
        {
            item->setIcon(QIcon(user.image()));
        }

    }

    connect(ui->loginButton, SIGNAL(released()), SLOT(onLoginButtonClicked()));
    connect(m_greeter, SIGNAL(authenticationComplete(bool)), SLOT(onAuthenticationComplete(bool)));
    connect(m_greeter, SIGNAL(showPrompt(QString)), SLOT(prompt(QString)));
//    QList<LdmSession> sessions = m_greeter->sessions();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onLoginButtonClicked()
{
    ui->feedbackLabel->setText("");
    if (ui->userList->currentItem())
    {
        m_greeter->startAuthentication(ui->userList->currentItem()->data(Qt::UserRole).toString());
    }
}

void Widget::onAuthenticationComplete(bool success)
{
    if (success)
    {
        ui->feedbackLabel->setText("YAY - log in");
        //        m_greeter->login(ui->userList->currentItem()->text(), "kde", "en-UK");
    }
    else
    {
        ui->feedbackLabel->setText("Sorry, you suck. Try again.");
    }
}

void Widget::prompt(QString message)
{
    qDebug() << message;
    m_greeter->provideSecret(ui->password->text());
}

Background::Background() :
    LdmGreeter()
{
    QLabel *background = new QLabel(this);
    //background->setPixmap(QPixmap("/home/david/.kde/share/wallpapers/nature_01__1920.png"));

    m_widget = new Widget(this);
    m_widget->move(this->width()/2 - m_widget->width()/2, this->height()/2 - m_widget->height()/2);
    m_widget->setAutoFillBackground(true);
}


Background::~Background()
{
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Background b;
    b.show();

//    QObject::connect(g, SIGNAL(quit()), &a, SLOT(closeAllWindows()));

    return a.exec();
}
