/*
    Floaties

    Copyright (c) 2009 by Nigmatullin Ruslan <euroelessar@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#define AVA_SIZE 24

#include <qutim/plugininterface.h>
extern uint qHash(const qutim_sdk_0_2::TreeModelItem &item);
#include <QWidget>
#include <QLabel>
#include <QContextMenuEvent>

class FloatiesPlugin;

class ContactWidget : public QWidget
{
    Q_OBJECT
public:
    ContactWidget(const qutim_sdk_0_2::TreeModelItem &item, const QString &name, FloatiesPlugin *plugin);
    ~ContactWidget();
    void setName(const QString &name);
    void setNewItem(const qutim_sdk_0_2::TreeModelItem &item);
    void setStatusIcon(const QIcon &icon);
    void setAvatar(const QIcon &icon);
    const qutim_sdk_0_2::TreeModelItem &getItem() { return m_item; }
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
//    virtual void paintEvent(QPaintEvent * event);
protected:
	virtual bool event(QEvent *event);
private:
    qutim_sdk_0_2::TreeModelItem m_item;
    QString m_name;
    QPixmap m_status;
    QPixmap m_avatar;
    QLabel *m_name_label;
    QLabel *m_status_label;
    QLabel *m_avatar_label;
    FloatiesPlugin *m_plugin;
    bool m_mouse_pressed;
    QPoint m_position;
};

#endif // CONTACTWIDGET_H
