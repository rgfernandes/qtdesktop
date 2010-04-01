# for mingw (windows)
#windows:LIBS += libws2_32
# for msvs (windows)
windows:LIBS += ws2_32.lib advapi32.lib
unix {
	LIBS += -lgnutls
	DEFINES += HAVE_GNUTLS
	mac {
		# the easiest way to obtain gnutls on mac is to use MacPorts (http://macports.org)
		INCLUDEPATH += /opt/local/include
		LIBS += -L/opt/local/lib
	}
}
win32 {
	LIBS += -llibgnutls-26
	DEFINES += HAVE_GNUTLS
}
# libidn
LIBIDN_BASE = $$PWD/libidn
CONFIG += libidn
include($$PWD/libidn.pro)

INCLUDEPATH += $$PWD/gloox
DEPENDPATH  += $$PWD/gloox

# Input
HEADERS += \
           $$PWD/gloox/adhoc.h \
           $$PWD/gloox/adhoccommandprovider.h \
           $$PWD/gloox/adhochandler.h \
           $$PWD/gloox/amp.h \
           $$PWD/gloox/annotations.h \
           $$PWD/gloox/annotationshandler.h \
           $$PWD/gloox/base64.h \
           $$PWD/gloox/bookmarkhandler.h \
           $$PWD/gloox/bookmarkstorage.h \
           $$PWD/gloox/bytestream.h \
           $$PWD/gloox/bytestreamdatahandler.h \
           $$PWD/gloox/bytestreamhandler.h \
           $$PWD/gloox/capabilities.h \
           $$PWD/gloox/chatstate.h \
           $$PWD/gloox/chatstatefilter.h \
           $$PWD/gloox/chatstatehandler.h \
           $$PWD/gloox/client.h \
           $$PWD/gloox/clientbase.h \
           $$PWD/gloox/component.h \
           $$PWD/gloox/compressionbase.h \
           $$PWD/gloox/compressiondatahandler.h \
           $$PWD/gloox/compressionzlib.h \
           $$PWD/gloox/connectionbase.h \
           $$PWD/gloox/connectionbosh.h \
           $$PWD/gloox/connectiondatahandler.h \
           $$PWD/gloox/connectionhandler.h \
           $$PWD/gloox/connectionhttpproxy.h \
           $$PWD/gloox/connectionlistener.h \
           $$PWD/gloox/connectionsocks5proxy.h \
           $$PWD/gloox/connectiontcpbase.h \
           $$PWD/gloox/connectiontcpclient.h \
           $$PWD/gloox/connectiontcpserver.h \
           $$PWD/gloox/connectiontls.h \
           $$PWD/gloox/dataform.h \
           $$PWD/gloox/dataformfield.h \
           $$PWD/gloox/dataformfieldcontainer.h \
           $$PWD/gloox/dataformitem.h \
           $$PWD/gloox/dataformreported.h \
           $$PWD/gloox/delayeddelivery.h \
           $$PWD/gloox/disco.h \
           $$PWD/gloox/discohandler.h \
           $$PWD/gloox/disconodehandler.h \
           $$PWD/gloox/dns.h \
           $$PWD/gloox/error.h \
           $$PWD/gloox/event.h \
           $$PWD/gloox/eventdispatcher.h \
           $$PWD/gloox/eventhandler.h \
           $$PWD/gloox/featureneg.h \
           $$PWD/gloox/flexoff.h \
           $$PWD/gloox/flexoffhandler.h \
           $$PWD/gloox/gloox.h \
           $$PWD/gloox/gpgencrypted.h \
           $$PWD/gloox/gpgsigned.h \
           $$PWD/gloox/inbandbytestream.h \
           $$PWD/gloox/instantmucroom.h \
           $$PWD/gloox/iq.h \
           $$PWD/gloox/iqhandler.h \
           $$PWD/gloox/jid.h \
           $$PWD/gloox/jingleaudiortp.h \
           $$PWD/gloox/jinglecontent.h \
           $$PWD/gloox/jingledescription.h \
           $$PWD/gloox/jingleiceudp.h \
           $$PWD/gloox/jingleplugin.h \
           $$PWD/gloox/jinglerawudp.h \
           $$PWD/gloox/jinglesession.h \
           $$PWD/gloox/jinglesessionhandler.h \
           $$PWD/gloox/jingletransport.h \
           $$PWD/gloox/jinglevideortp.h \
           $$PWD/gloox/lastactivity.h \
           $$PWD/gloox/lastactivityhandler.h \
           $$PWD/gloox/loghandler.h \
           $$PWD/gloox/logsink.h \
           $$PWD/gloox/macros.h \
           $$PWD/gloox/md5.h \
           $$PWD/gloox/message.h \
           $$PWD/gloox/messageevent.h \
           $$PWD/gloox/messageeventfilter.h \
           $$PWD/gloox/messageeventhandler.h \
           $$PWD/gloox/messagefilter.h \
           $$PWD/gloox/messagehandler.h \
           $$PWD/gloox/messagesession.h \
           $$PWD/gloox/messagesessionhandler.h \
           $$PWD/gloox/mucinvitationhandler.h \
           $$PWD/gloox/mucmessagesession.h \
           $$PWD/gloox/mucroom.h \
           $$PWD/gloox/mucroomconfighandler.h \
           $$PWD/gloox/mucroomhandler.h \
           $$PWD/gloox/mutex.h \
           $$PWD/gloox/mutexguard.h \
           $$PWD/gloox/nickname.h \
           $$PWD/gloox/nonsaslauth.h \
           $$PWD/gloox/oob.h \
           $$PWD/gloox/parser.h \
           $$PWD/gloox/prep.h \
           $$PWD/gloox/presence.h \
           $$PWD/gloox/presencehandler.h \
           $$PWD/gloox/privacyitem.h \
           $$PWD/gloox/privacylisthandler.h \
           $$PWD/gloox/privacymanager.h \
           $$PWD/gloox/privatexml.h \
           $$PWD/gloox/privatexmlhandler.h \
           $$PWD/gloox/pubsub.h \
           $$PWD/gloox/pubsubevent.h \
           $$PWD/gloox/pubsubeventhandler.h \
           $$PWD/gloox/pubsubitem.h \
           $$PWD/gloox/pubsubmanager.h \
           $$PWD/gloox/pubsubresulthandler.h \
           $$PWD/gloox/receipt.h \
           $$PWD/gloox/registration.h \
           $$PWD/gloox/registrationhandler.h \
           $$PWD/gloox/resource.h \
           $$PWD/gloox/rosteritem.h \
           $$PWD/gloox/rosteritemdata.h \
           $$PWD/gloox/rosterlistener.h \
           $$PWD/gloox/rostermanager.h \
           $$PWD/gloox/search.h \
           $$PWD/gloox/searchhandler.h \
           $$PWD/gloox/sha.h \
           $$PWD/gloox/shim.h \
           $$PWD/gloox/sihandler.h \
           $$PWD/gloox/simanager.h \
           $$PWD/gloox/siprofileft.h \
           $$PWD/gloox/siprofilefthandler.h \
           $$PWD/gloox/siprofilehandler.h \
           $$PWD/gloox/socks5bytestream.h \
           $$PWD/gloox/socks5bytestreammanager.h \
           $$PWD/gloox/socks5bytestreamserver.h \
           $$PWD/gloox/stanza.h \
           $$PWD/gloox/stanzaextension.h \
           $$PWD/gloox/stanzaextensionfactory.h \
           $$PWD/gloox/statisticshandler.h \
           $$PWD/gloox/subscription.h \
           $$PWD/gloox/subscriptionhandler.h \
           $$PWD/gloox/tag.h \
           $$PWD/gloox/taghandler.h \
           $$PWD/gloox/tlsbase.h \
           $$PWD/gloox/tlsdefault.h \
           $$PWD/gloox/tlsgnutlsbase.h \
           $$PWD/gloox/tlsgnutlsclient.h \
           $$PWD/gloox/tlsgnutlsclientanon.h \
           $$PWD/gloox/tlsgnutlsserveranon.h \
           $$PWD/gloox/tlshandler.h \
           $$PWD/gloox/tlsopenssl.h \
           $$PWD/gloox/tlsschannel.h \
           $$PWD/gloox/uniquemucroom.h \
           $$PWD/gloox/util.h \
           $$PWD/gloox/vcard.h \
           $$PWD/gloox/vcardhandler.h \
           $$PWD/gloox/vcardmanager.h \
           $$PWD/gloox/vcardupdate.h \
           $$PWD/gloox/xhtmlim.h
SOURCES += \
           $$PWD/gloox/adhoc.cpp \
           $$PWD/gloox/amp.cpp \
           $$PWD/gloox/annotations.cpp \
           $$PWD/gloox/base64.cpp \
           $$PWD/gloox/bookmarkstorage.cpp \
           $$PWD/gloox/capabilities.cpp \
           $$PWD/gloox/chatstate.cpp \
           $$PWD/gloox/chatstatefilter.cpp \
           $$PWD/gloox/client.cpp \
           $$PWD/gloox/clientbase.cpp \
           $$PWD/gloox/component.cpp \
           $$PWD/gloox/compressionzlib.cpp \
           $$PWD/gloox/connectionbosh.cpp \
           $$PWD/gloox/connectionhttpproxy.cpp \
           $$PWD/gloox/connectionsocks5proxy.cpp \
           $$PWD/gloox/connectiontcpbase.cpp \
           $$PWD/gloox/connectiontcpclient.cpp \
           $$PWD/gloox/connectiontcpserver.cpp \
           $$PWD/gloox/connectiontls.cpp \
           $$PWD/gloox/dataform.cpp \
           $$PWD/gloox/dataformfield.cpp \
           $$PWD/gloox/dataformfieldcontainer.cpp \
           $$PWD/gloox/dataformitem.cpp \
           $$PWD/gloox/dataformreported.cpp \
           $$PWD/gloox/delayeddelivery.cpp \
           $$PWD/gloox/disco.cpp \
           $$PWD/gloox/dns.cpp \
           $$PWD/gloox/error.cpp \
           $$PWD/gloox/eventdispatcher.cpp \
           $$PWD/gloox/featureneg.cpp \
           $$PWD/gloox/flexoff.cpp \
           $$PWD/gloox/gloox.cpp \
           $$PWD/gloox/gpgencrypted.cpp \
           $$PWD/gloox/gpgsigned.cpp \
           $$PWD/gloox/inbandbytestream.cpp \
           $$PWD/gloox/instantmucroom.cpp \
           $$PWD/gloox/iq.cpp \
           $$PWD/gloox/jid.cpp \
           $$PWD/gloox/jinglecontent.cpp \
           $$PWD/gloox/jingledescription.cpp \
           $$PWD/gloox/jinglesession.cpp \
           $$PWD/gloox/jingletransport.cpp \
           $$PWD/gloox/lastactivity.cpp \
           $$PWD/gloox/logsink.cpp \
           $$PWD/gloox/md5.cpp \
           $$PWD/gloox/message.cpp \
           $$PWD/gloox/messageevent.cpp \
           $$PWD/gloox/messageeventfilter.cpp \
           $$PWD/gloox/messagefilter.cpp \
           $$PWD/gloox/messagesession.cpp \
           $$PWD/gloox/mucmessagesession.cpp \
           $$PWD/gloox/mucroom.cpp \
           $$PWD/gloox/mutex.cpp \
           $$PWD/gloox/nickname.cpp \
           $$PWD/gloox/nonsaslauth.cpp \
           $$PWD/gloox/oob.cpp \
           $$PWD/gloox/parser.cpp \
           $$PWD/gloox/prep.cpp \
           $$PWD/gloox/presence.cpp \
           $$PWD/gloox/privacyitem.cpp \
           $$PWD/gloox/privacymanager.cpp \
           $$PWD/gloox/privatexml.cpp \
           $$PWD/gloox/pubsubevent.cpp \
           $$PWD/gloox/pubsubitem.cpp \
           $$PWD/gloox/pubsubmanager.cpp \
           $$PWD/gloox/receipt.cpp \
           $$PWD/gloox/registration.cpp \
           $$PWD/gloox/rosteritem.cpp \
           $$PWD/gloox/rostermanager.cpp \
           $$PWD/gloox/search.cpp \
           $$PWD/gloox/sha.cpp \
           $$PWD/gloox/shim.cpp \
           $$PWD/gloox/simanager.cpp \
           $$PWD/gloox/siprofileft.cpp \
           $$PWD/gloox/socks5bytestream.cpp \
           $$PWD/gloox/socks5bytestreammanager.cpp \
           $$PWD/gloox/socks5bytestreamserver.cpp \
           $$PWD/gloox/stanza.cpp \
           $$PWD/gloox/stanzaextensionfactory.cpp \
           $$PWD/gloox/subscription.cpp \
           $$PWD/gloox/tag.cpp \
           $$PWD/gloox/tlsdefault.cpp \
           $$PWD/gloox/tlsgnutlsbase.cpp \
           $$PWD/gloox/tlsgnutlsclient.cpp \
           $$PWD/gloox/tlsgnutlsclientanon.cpp \
           $$PWD/gloox/tlsgnutlsserveranon.cpp \
           $$PWD/gloox/tlsopenssl.cpp \
           $$PWD/gloox/tlsschannel.cpp \
           $$PWD/gloox/uniquemucroom.cpp \
           $$PWD/gloox/util.cpp \
           $$PWD/gloox/vcard.cpp \
           $$PWD/gloox/vcardmanager.cpp \
           $$PWD/gloox/vcardupdate.cpp \
           $$PWD/gloox/xhtmlim.cpp
