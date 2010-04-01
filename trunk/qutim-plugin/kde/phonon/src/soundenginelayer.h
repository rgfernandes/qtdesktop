/*
	SoundEngineLayer

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

#ifndef SOUNDENGINELAYER_H
#define SOUNDENGINELAYER_H

#include <QObject>
#include <QString>
#include <qutim/layerscity.h>
#include <phonon/mediasource.h>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/backendcapabilities.h>

//#include <phonon/mediasource.h>
//#include <phonon/mediaobject.h>
//#include <phonon/audiooutput.h>
////#include <phonon/experimental/avcapture.h>
////#include <phonon/experimental/audiodataoutput.h>
//#include <phonon/backendcapabilities.h>
//#include <phonon/abstractmediastream.h>
#include <QBuffer>

using namespace qutim_sdk_0_2;
using namespace Phonon;

class SoundEngineLayerImpl : public QObject, public SoundEngineLayerInterface
{
    Q_OBJECT

public:
    SoundEngineLayerImpl();

	virtual bool init(PluginSystemInterface *plugin_system);
    virtual void release() {}
	virtual void setProfileName(const QString &profile_name);
	virtual void setLayerInterface( LayerType type, LayerInterface *layer) { LayersCity::instance().setLayerInterface( type, layer ); }

    virtual void saveLayerSettings() {}
    virtual QList<SettingsStructure> getLayerSettingsList() { return m_settings; }
    virtual void removeLayerSettings() {}

    virtual void saveGuiSettingsPressed() {}
    virtual QList<SettingsStructure> getGuiSettingsList() { return m_gui_settings; }
    virtual void removeGuiLayerSettings() {}
    virtual void playSound(QIODevice *device);
	virtual void playSound(const QString &filename);
    virtual QIODevice *grabSound();
public slots:
    void killDevice();
	void stopRecord();
//	void dataReady(const QMap<Phonon::Experimental::AudioDataOutput::Channel, QVector<qint16> > &data);
private:
	void loadSettings();
	void playSound(const MediaSource &source);
//	Experimental::AvCapture m_capture;
    QString m_profile_name;
//	QBuffer m_buffer;
};

#endif // SOUNDENGINELAYER_H
