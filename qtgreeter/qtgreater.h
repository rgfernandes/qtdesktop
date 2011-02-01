#ifndef QTGREATER_H
#define QTGREATER_H

#include <QWidget>

#include <lightdm/ldmgreeter.h>

#include "ui_qtgreater.h"

//class LdmGreeter;

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(LdmGreeter*, QWidget *parent = 0);
    ~Widget();

private slots:
    void onLoginButtonClicked();
    void onAuthenticationComplete(bool success);
    void prompt(QString message);

private:
    Ui::Widget *ui;
    LdmGreeter* m_greeter;
};

//FIXME background is now the greeter manager.

//class Widget;

class Background : public LdmGreeter
{
    Q_OBJECT
public:
    explicit Background();
    ~Background();

private:
    Widget* m_widget;
};


#endif // QTGREATER_H
