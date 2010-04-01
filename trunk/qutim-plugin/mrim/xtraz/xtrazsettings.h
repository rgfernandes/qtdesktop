#ifndef XTRAZSETTINGS_H
#define XTRAZSETTINGS_H

#include <QtGui/QWidget>

namespace Ui {
    class XtrazSettings;
}

class XtrazPackage;

class XtrazSettings : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(XtrazSettings)
public:
    explicit XtrazSettings(QString aProfileName, QWidget *parent = 0);
    virtual ~XtrazSettings();
    void SaveSettings();
protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::XtrazSettings *m_ui;    
    QString m_profileName;
};

#endif // XTRAZSETTINGS_H
