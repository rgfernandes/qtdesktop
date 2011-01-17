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

#include "contactwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include "floaties.h"
#include <QDebug>
#include <QToolTip>

ContactWidget::ContactWidget(const qutim_sdk_0_2::TreeModelItem &item, const QString &name, FloatiesPlugin *plugin)
    : m_item(item), m_name(name), m_plugin(plugin), m_mouse_pressed(false), m_position(0,0)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    m_status_label = new QLabel();
    m_name_label = new QLabel(m_name.isEmpty()?m_item.m_item_name:m_name);
    m_avatar_label = new QLabel();
    layout->addWidget(m_avatar_label);
    layout->addWidget(m_name_label);
    layout->addWidget(m_status_label);
#if defined(Q_OS_MAC)
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
#else
        setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip);
#endif
	setAttribute(Qt::WA_AlwaysShowToolTips, true);
    setWindowOpacity(0.8);
//    setFixedSize(QSize(150, 26));
}

ContactWidget::~ContactWidget()
{
}

//void ContactWidget::paintEvent(QPaintEvent * event)
//{
//}

void ContactWidget::setName(const QString &name)
{
    m_name = name;
    m_name_label->setText(m_name.isEmpty()?m_item.m_item_name:m_name);
}

void ContactWidget::setNewItem(const qutim_sdk_0_2::TreeModelItem &item)
{
    m_item = item;
    m_name_label->setText(m_name.isEmpty()?m_item.m_item_name:m_name);
}

void ContactWidget::setStatusIcon(const QIcon &icon)
{
    m_status = icon.pixmap(QSize(65535, 65535));
    m_status_label->setPixmap(m_status);
}

void ContactWidget::setAvatar(const QIcon &icon)
{
    QSize ava_size(AVA_SIZE,AVA_SIZE);
    QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(65535,65535)),QIcon::Normal,QIcon::On);
    if(!pixmap.isNull())
    {
        QPixmap alpha (ava_size);
        alpha.fill(QColor(0,0,0));
        QPainter painter(&alpha);
        QPen pen(QColor(127,127,127));
        painter.setRenderHint(QPainter::Antialiasing);
        pen.setWidth(0);
        painter.setPen(pen);
        painter.setBrush(QBrush(QColor(255,255,255)));
        painter.drawRoundedRect(QRectF(QPointF(0,0),QSize(AVA_SIZE-1,AVA_SIZE-1)),5,5);
        painter.end();
        pixmap = pixmap.scaled(ava_size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        pixmap.setAlphaChannel(alpha);
    }
    m_avatar = pixmap;
    m_avatar_label->setPixmap(m_avatar);
}

void ContactWidget::contextMenuEvent(QContextMenuEvent * event)
{
    m_plugin->contextMenu(m_item, event->globalPos());
}

void ContactWidget::mousePressEvent(QMouseEvent *event)
{
    m_mouse_pressed = event->button() == Qt::LeftButton;
    m_position = event->pos();
}

void ContactWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_mouse_pressed = false;
}

void ContactWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_mouse_pressed)
        move(event->globalPos() - m_position);
}

void ContactWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	Q_UNUSED(event);
    m_plugin->createChat(m_item);
}

void ContactWidget::wheelEvent(QWheelEvent *event)
{
    setWindowOpacity(windowOpacity()+event->delta()/2400.0);
}

bool ContactWidget::event(QEvent *event)
{
	if(event->type() == QEvent::ToolTip)
	{
		QString tool_tip = SystemsCity::PluginSystem()->getItemToolTip(m_item);
		if (!tool_tip.isEmpty())
			QToolTip::showText(static_cast<QHelpEvent*>(event)->globalPos(), tool_tip, this);
		else
			event->ignore();
		return true;
	}
	return QWidget::event(event);
}
