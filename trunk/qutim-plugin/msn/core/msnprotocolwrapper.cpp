/*
    MSNProtocolWrapper

    Copyright (c) 2009 by Rustam Chakin <qutim.develop@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

#include "msnprotocolwrapper.h"
#include "msnsocket.h"
#include <QDateTime>


MSNProtocolWrapper::MSNProtocolWrapper(QObject *parent):
	QObject(parent)
{
    mainConnection = 0;
}

MSNProtocolWrapper::~MSNProtocolWrapper()
{
    tryToDisconnect();
    qDeleteAll(m_socket_list);
}

QString MSNProtocolWrapper::fromStd(const std::string &str)
{
	return QString::fromUtf8(str.c_str());
}

std::string MSNProtocolWrapper::toStd(const QString &str)
{
	return str.toUtf8().constData();
}

void MSNProtocolWrapper::tryToConnect(const QString &username, const QString &pass)
{

    if(!mainConnection)
    {
		qDebug() << username << pass;
	MSN::Passport uname(toStd(username));
	mainConnection = new MSN::NotificationServerConnection(uname, toStd(pass), this);
    }
	mainConnection->connect("messenger.hotmail.com", 1863);

}

void MSNProtocolWrapper::tryToDisconnect()
{
    MSNSocket *a = 0;

    if (mainConnection)
    {
	QListIterator<MSNSocket *> i(m_socket_list);
	while (i.hasNext())
	{
	    a = i.next();
	    QObject::disconnect (a, 0, 0, 0);
	    m_socket_list.removeAll (a);

	}
	m_socket_list.clear ();

	if (mainConnection->connectionState () !=
	    MSN::NotificationServerConnection::NS_DISCONNECTED)
	{
	    delete mainConnection;
	    mainConnection = NULL;
	}
    }
}


void MSNProtocolWrapper::emitSocketError( QAbstractSocket::SocketError error )
{
    if ( !mainConnection )
	return;

    MSNSocket* socket = qobject_cast<MSNSocket*>(sender());
    Q_ASSERT( socket );

    MSN::Connection *c = mainConnection->connectionWithSocket((void*)socket);
    if ( !c )
	return;

    if ( c == mainConnection )
	qDebug()<<"SOCK ERROR:"<<error;
	//emit socketError( error );
    else
	c->disconnect();
}


void MSNProtocolWrapper::setStatus(MSN::BuddyStatus status)
{
    if(mainConnection)
    {
	uint clientid=0;

	clientid += MSN::MSNC7;
	clientid += MSN::MSNC6;
	clientid += MSN::MSNC5;
	clientid += MSN::MSNC4;
	clientid += MSN::MSNC3;
	clientid += MSN::MSNC2;
	clientid += MSN::MSNC1;
	clientid += MSN::SupportWinks;
	clientid += MSN::VoiceClips;
	clientid += MSN::InkGifSupport;
	clientid += MSN::SIPInvitations;
	clientid += MSN::SupportMultiPacketMessaging;

	mainConnection->setState(status, clientid);
    }
}

void MSNProtocolWrapper::setStatusMessage(const QString &message)
{
    MSN::personalInfo info;
    info.PSM = toStd(message);
    mainConnection->setPersonalStatus(info);
}


void MSNProtocolWrapper::sendMessage(const QString &buddy_id, const QString &message)
{

	const std::string rcpt_ = toStd(buddy_id);
	const std::string msg_ = toStd(message);
	const std::pair<std::string,std::string> *ctx = new std::pair<std::string,std::string>(rcpt_, msg_);

	// request a new switchboard connection
	mainConnection->requestSwitchboardConnection (ctx);
}

void MSNProtocolWrapper::requestSwitchBoard(const QString &buddy_id)
{
    const std::string *rcpt_ = new std::string(toStd(buddy_id));
    mainConnection->requestSwitchboardConnection (rcpt_);
}

//CALLBACK FUNCTIONS

void MSNProtocolWrapper::registerSocket(void *s, int reading, int writing, bool isSSL)
{

    Q_UNUSED( writing );
    Q_UNUSED( isSSL );

    MSNSocket *a = (MSNSocket*)s;
    if (!a)
	return;

    if (reading)
    {
	QObject::disconnect(a, SIGNAL (readyRead ()),0,0);
	QObject::connect (a, SIGNAL (readyRead ()), a,
		  SLOT (incomingData ()));
    }

}

int
MSNProtocolWrapper::getSocketFileDescriptor (void *sock)
{
    MSNSocket *a = (MSNSocket*)sock;
    if(!a)
	return -1;
    return a->socketDescriptor();
}

void MSNProtocolWrapper::closeSocket(void *s)
{
    MSNSocket *a = (MSNSocket*)s;
    if (a)
    {
	a->close ();
	m_socket_list.removeAll (a);
	a->deleteLater();
    }
}

void MSNProtocolWrapper::unregisterSocket(void *s)
{
    MSNSocket *a = (MSNSocket*)s;
    if (a)
    {
	QObject::disconnect(a, SIGNAL (readyRead ()),0,0);
    }
}

void MSNProtocolWrapper::gotFriendlyName(MSN::NotificationServerConnection * conn, std::string friendlyname)
{
    printf("Your friendlyname is now: %s\n", friendlyname.c_str());
}

void MSNProtocolWrapper::gotBuddyListInfo(MSN::NotificationServerConnection * conn, MSN::ListSyncInfo * info)
{

   // IMPORTANT
    // Here you need to fill a vector with all your contacts
    // both received by the server and previous ones.
    // Next pass this vector to the function completeConnection()
    // if you do not call completeConnection(), the service will
    // not work.
    std::map < std::string, MSN::Buddy * >::iterator i =
	info->contactList.begin ();
    std::map < std::string, int >allContacts;

    for (; i != info->contactList.end (); ++i)
    {
	MSN::Buddy * contact = (*i).second;
	if (contact->lists & MSN::LST_AB       // only if it is the address book
	   && contact->properties["isMessengerUser"] == "true")
	{
	    allContacts[contact->userName.c_str ()] = 0;
	    allContacts[contact->userName.c_str ()] |= MSN::LST_AB;
	    std::list < MSN::Buddy::PhoneNumber >::iterator pns =
		contact->phoneNumbers.begin ();
	    std::list < MSN::Group * >::iterator g = contact->groups.begin ();
	}
	if (contact->lists & MSN::LST_AL)
	{
	    allContacts[contact->userName.c_str ()] |= MSN::LST_AL;
	}

	if (contact->lists & MSN::LST_BL)
	{
	    allContacts[contact->userName.c_str ()] |= MSN::LST_BL;
	}

	if (contact->lists & MSN::LST_RL)
	{
	    //printf ("-RL %s \n", contact->userName.c_str ());
	}
	if (contact->lists & MSN::LST_PL)
	{
	    //printf ("-PL %s \n", contact->userName.c_str ());
	}
    }
    //printf ("Available Groups:\n");
    std::map < std::string, MSN::Group >::iterator g = info->groups.begin ();

    for (; g != info->groups.end (); g++)
    {
	//printf ("    %s: %s\n", (*g).second.groupID.c_str (),
	//        (*g).second.name.c_str ());
    }

    // this will send the ADL command to the server
    // It is necessary. Do not forget to add *all* your contacts to allContacts,
    // (both Forward, allow and block lists) or you probably will
    // loose someone.
    // A contact cannot be present both on allow and block lists or the
    // server will return an error, so you need to let your application
    // choose the better list to put it in.
    //m_server->m_account->groupListReceivedFromServer (info->groups);
    //m_server->m_account->addressBookReceivedFromServer (info->contactList);
    emit groupsArriving(info->groups);
    emit buddiesArriving(info->contactList);
    mainConnection->completeConnection (allContacts, info);


}

void MSNProtocolWrapper::gotLatestListSerial(MSN::NotificationServerConnection * conn, std::string lastChange)
{
    // The application needs to track this number to not ask for the whole contact
    // list every login
    printf("The latest change number is: %s\n", lastChange.c_str());
}

void MSNProtocolWrapper::gotGTC(MSN::NotificationServerConnection * conn, char c)
{
    printf("Your GTC value is now %c\n", c);
}

void MSNProtocolWrapper::gotOIMDeleteConfirmation(MSN::NotificationServerConnection * conn, bool success, std::string id)
{
    if(success)
	std::cout << "OIM "<< id << " removed sucessfully." << std::endl;
    else
	std::cout << "OIM "<< id << " not removed sucessfully." << std::endl;

}

void MSNProtocolWrapper::gotOIMSendConfirmation(MSN::NotificationServerConnection * conn, bool success, int id)
{
    if(success)
	std::cout << "OIM " << id << " sent sucessfully." << std::endl;
    else
	std::cout << "OIM " << id << " not sent sucessfully." << std::endl;
}

void MSNProtocolWrapper::gotOIM(MSN::NotificationServerConnection * conn, bool success, std::string id, std::string message)
{
    if(success)
	std::cout << "ID: " << id << std::endl << "\t" << message << std::endl;
    else
	std::cout << "Error retreiving OIM " << id << std::endl;
}

void MSNProtocolWrapper::gotOIMList(MSN::NotificationServerConnection * conn, std::vector<MSN::eachOIM> OIMs)
{
    if(OIMs.size()==0)
    {
	printf("No Offline messages\n");
	return;
    }
    std::vector<MSN::eachOIM>::iterator i = OIMs.begin();
    for(; i<OIMs.end();i++)
    {
	printf("Offline message from: %s\n\t - Friendly Name: %s\n\t - Id: %s\n",(*i).from.c_str(), (*i).fromFN.c_str(), (*i).id.c_str());
    }
}

void MSNProtocolWrapper::connectionReady(MSN::Connection * conn)
{
    emit iAmConnected();
}

void MSNProtocolWrapper::gotBLP(MSN::NotificationServerConnection * conn, char c)
{
    printf("Your BLP value is now %cL\n", c);
}

void MSNProtocolWrapper::addedListEntry(MSN::NotificationServerConnection * conn, MSN::ContactList list, MSN::Passport username, std::string friendlyname)
{
    if(list == MSN::LST_RL)
	// after adding the user you need to delete it from the pending list.
	// it will be added automatically by the msn service
	printf("%s is now on your list %d. FriendlyName: %s\n", username.c_str(), list, friendlyname.c_str());
    else
	// on normal lists you'll never receive the contacts displayname
	// it is not needed anyway
	printf("%s is now on your list %d\n", username.c_str(), list);
}

void MSNProtocolWrapper::removedListEntry(MSN::NotificationServerConnection * conn, MSN::ContactList list, MSN::Passport username)
{
    // list is a number which matches with MSN::ContactList enum on util.h
    printf("%s has been removed from list %d\n", username.c_str(), list);
}

void MSNProtocolWrapper::addedGroup(MSN::NotificationServerConnection * conn, bool added, std::string groupName, std::string groupID)
{
    if(added)
	printf("A group named %s (%s) was added\n", groupName.c_str(), groupID.c_str());
    else
	printf("Group (%s) was NOT added\n", groupName.c_str());
}

void MSNProtocolWrapper::removedGroup(MSN::NotificationServerConnection * conn, bool removed, std::string groupID)
{
    if(removed)
	printf("A group with ID %s was removed\n", groupID.c_str());
    else
    printf("Group (%s) was NOT removed\n", groupID.c_str());
}

void MSNProtocolWrapper::renamedGroup(MSN::NotificationServerConnection * conn, bool renamed, std::string newGroupName, std::string groupID)
{
    if(renamed)
	printf("A group with ID %s was renamed to %s\n", groupID.c_str(), newGroupName.c_str());
    else
	printf("A group with ID %s was NOT renamed to %s\n", groupID.c_str(), newGroupName.c_str());
}

void MSNProtocolWrapper::showError(MSN::Connection * conn, std::string msg)
{
    Q_UNUSED( conn );
    std::cout << "MSN: Error: " << msg.c_str () << std::endl;
    QString a = msg.c_str ();
    // FIXME
    if (a.contains ("Wrong Password"))
    {
	//emit wrongPassword ();
    }
}

void MSNProtocolWrapper::buddyChangedStatus(MSN::NotificationServerConnection * conn, MSN::Passport buddy, std::string friendlyname, MSN::BuddyStatus status, unsigned int clientID, std::string msnobject)
{
    emit buddyChangeStatus(buddy.c_str(),friendlyname.c_str(),status);
    //printf("%s (%s) is now %s\n", friendlyname.c_str(), buddy.c_str(), MSN::buddyStatusToString(status).c_str());
    /*if (clientID & MSN::SupportWebcam)
	printf("\t Supports Webcam\n");
    if (clientID & MSN::VoiceClips)
	printf("\t Supports VoiceClips\n");
    if (clientID & MSN::SupportDirectIM)
	printf("\t Supports DirectIM\n");
    if (clientID & MSN::SharingFolders)
	printf("\t Supports SharingFolders\n");
    if (clientID & MSN::MSNC1)
	printf("\t Supports MSNC1\n");
    if (clientID & MSN::MSNC2)
	printf("\t Supports MSNC2\n");
    if (clientID & MSN::MSNC3)
	printf("\t Supports MSNC3\n");
    if (clientID & MSN::MSNC4)
	printf("\t Supports MSNC4\n");
    if (clientID & MSN::MSNC5)
	printf("\t Supports MSNC5\n");
    if (clientID & MSN::MSNC6)
	printf("\t Supports MSNC6\n");
    if (clientID & MSN::MSNC7)
	printf("\t Supports MSNC7\n");
    if (clientID & MSN::InkGifSupport)
	printf("\t Supports Ink Gif\n");
    if (clientID & MSN::InkIsfSupport)
	printf("\t Supports Ink Isf\n");*/

//  lastObject = msnobject;
}

void MSNProtocolWrapper::buddyOffline(MSN::NotificationServerConnection * conn, MSN::Passport buddy)
{
    //printf("%s is now offline\n", buddy.c_str());
    emit buddyOffline(buddy.c_str());
}

void MSNProtocolWrapper::gotSwitchboard(MSN::SwitchboardServerConnection * conn, const void * tag)
{
    if (tag){
	//const std::pair<std::string, std::string> *ctx = static_cast<const std::pair<std::string, std::string> *>(tag);
	const std::string *buddy_ = static_cast<const std::string*>(tag);
	conn->inviteUser(MSN::Passport(buddy_->c_str()));
    }
}

void MSNProtocolWrapper::buddyJoinedConversation(MSN::SwitchboardServerConnection * conn, MSN::Passport username, std::string friendlyname, int is_initial)
{
    if (conn->auth.tag){
	//const std::pair<std::string, std::string> *ctx = static_cast<const std::pair<std::string, std::string> *>(conn->auth.tag);
	const std::string *buddy_ = static_cast<const std::string*>(conn->auth.tag);
//	int trid = conn->sendMessage(ctx->second);
	emit switchBoardCreated(buddy_->c_str(),conn);
	delete buddy_;
	conn->auth.tag = NULL;

    }
}

void MSNProtocolWrapper::buddyLeftConversation(MSN::SwitchboardServerConnection * conn, MSN::Passport username)
{
    printf("%s has now left the session\n", username.c_str());
}

void MSNProtocolWrapper::gotInstantMessage(MSN::SwitchboardServerConnection * conn, MSN::Passport username, std::string friendlyname, MSN::Message * msg)
{
//    printf("--- Message from %s (%s) in font %s:\n%s\n", friendlyname.c_str(), username.c_str(), msg->getFontName().c_str(), msg->getBody().c_str());
    emit messageArrived(username.c_str(), friendlyname.c_str(), msg->getBody().c_str(), QDateTime::currentDateTime(),conn);
}

void MSNProtocolWrapper::gotEmoticonNotification(MSN::SwitchboardServerConnection * conn, MSN::Passport username, std::string alias, std::string msnobject)
{
    printf("--- Emoticon '%s' from %s -> %s\n", alias.c_str(), username.c_str(), msnobject.c_str());
    //conn->requestEmoticon(sessionID++, filename2, msnobject, alias);
}

void MSNProtocolWrapper::failedSendingMessage(MSN::Connection * conn)
{
    printf("**************************************************\n");
    printf("ERROR:  Your last message failed to send correctly\n");
    printf("**************************************************\n");
}

void MSNProtocolWrapper::buddyTyping(MSN::SwitchboardServerConnection * conn, MSN::Passport username, std::string friendlyname)
{
    printf("\t%s (%s) is typing...\n", friendlyname.c_str(), username.c_str());

}

void MSNProtocolWrapper::gotNudge(MSN::SwitchboardServerConnection * conn, MSN::Passport username)
{
    printf("\t%s sent you a nudge ...\n", username.c_str());
}

void MSNProtocolWrapper::gotVoiceClipNotification(MSN::SwitchboardServerConnection * conn, MSN::Passport username, std::string msnobject)
{
    printf("\t%s sent you a voice clip...\n", username.c_str());
    //conn->requestVoiceClip(sessionID++, filename2, msnobject);
}

void MSNProtocolWrapper::gotWinkNotification(MSN::SwitchboardServerConnection * conn, MSN::Passport username, std::string msnobject)
{
    printf("\t%s sent you a Wink...\n", username.c_str());
    // you should generate a random sessionID number
    //conn->requestWink(sessionID++, filename2, msnobject);
}

void MSNProtocolWrapper::gotInk(MSN::SwitchboardServerConnection * conn, MSN::Passport username, std::string image)
{
    // image variable is the base64 encoded gif file
    printf("\t%s sent you an Ink...\n", username.c_str());
//  std::string filename2("/tmp/ink.bin"+MSN::toStr(sessionID));
    // you should generate a random sessionID number
//  conn->requestFile(sessionID++, filename2, msnobject);
}

void MSNProtocolWrapper::gotContactDisplayPicture(MSN::SwitchboardServerConnection * conn, MSN::Passport passport, std::string filename )
{
    printf("Received display picture from %s. File: %s\n", passport.c_str(), filename.c_str());
}

void MSNProtocolWrapper::gotActionMessage(MSN::SwitchboardServerConnection * conn, MSN::Passport username, std::string message)
{
    printf("\t%s sent you an action message: %s\n", username.c_str(), message.c_str());
}

void MSNProtocolWrapper::gotInitialEmailNotification(MSN::NotificationServerConnection * conn, int msgs_inbox, int unread_inbox, int msgs_folders, int unread_folders)
{
    if (unread_inbox > 0)
	printf("You have %d new messages in your Inbox. Total: %d\n", unread_inbox, msgs_inbox);

    if (unread_folders > 0)
	printf("You have %d new messages in other folders. Total: %d\n", unread_folders, msgs_folders);
}

void MSNProtocolWrapper::gotNewEmailNotification(MSN::NotificationServerConnection * conn, std::string from, std::string subject)
{
    printf("New e-mail has arrived from %s.\nSubject: %s\n", from.c_str(), subject.c_str());
}

void MSNProtocolWrapper::fileTransferInviteResponse(MSN::SwitchboardServerConnection * conn, unsigned int sessionID, bool response)
{
    if(response)
	    printf("Session accepted %d: \n", sessionID);
    else
	    printf("Session not accepted %d: \n", sessionID);
}

void MSNProtocolWrapper::fileTransferProgress(MSN::SwitchboardServerConnection * conn, unsigned int sessionID, unsigned long long transferred, unsigned long long total)
{
    printf("File transfer: session %d\t(%llu/%llu bytes sent/received)\n", sessionID, transferred, total);
}

void MSNProtocolWrapper::fileTransferFailed(MSN::SwitchboardServerConnection * conn, unsigned int sessionID, MSN::fileTransferError error)
{
    if( error == MSN::FILE_TRANSFER_ERROR_USER_CANCELED)
	printf("The other user canceled the file transfer failed. Session: %d\n", sessionID);
    if( error == MSN::FILE_TRANSFER_ERROR_UNKNOWN)
	printf("File transfer failed. Session: %d\n", sessionID);
}

void MSNProtocolWrapper::fileTransferSucceeded(MSN::SwitchboardServerConnection * conn, unsigned int sessionID)
{
    printf("File transfer successfully completed. session: %d\n", sessionID);
}

void MSNProtocolWrapper::gotNewConnection(MSN::Connection * conn)
{
    if (dynamic_cast < MSN::NotificationServerConnection * >(conn))
	dynamic_cast <MSN::NotificationServerConnection *>(conn)->synchronizeContactList ();
}

void MSNProtocolWrapper::buddyChangedPersonalInfo(MSN::NotificationServerConnection * conn, MSN::Passport fromPassport, MSN::personalInfo pInfo)
{
    // MSN::personalInfo shows all the data you can grab from the contact
    printf("User %s Personal Message: %s\n", fromPassport.c_str(),pInfo.PSM.c_str());
}

void MSNProtocolWrapper::closingConnection(MSN::Connection * conn)
{
    emit iAmDisconnected();
    //printf("Closed connection with socket %d\n", conn->sock);
}

void MSNProtocolWrapper::changedStatus(MSN::NotificationServerConnection * conn, MSN::BuddyStatus state)
{

}

void * MSNProtocolWrapper::connectToServer(std::string hostname, int port, bool *connected, bool isSSL)
{
    //isSSL = false;
    MSNSocket *a = new MSNSocket(mainConnection, isSSL);
    if(!a)
	return NULL;

    connect( a, SIGNAL(sslErrors(const QList<QSslError> &)), a, SLOT(ignoreSslErrors()) );
    connect( a, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(emitSocketError(QAbstractSocket::SocketError)) );

    if(!isSSL)
	a->connectToHost (hostname.c_str (), port);
    else
	a->connectToHostEncrypted (hostname.c_str (), port);

    *connected = false;
    m_socket_list.append (a);
    return (void*)a;
}

int MSNProtocolWrapper::listenOnPort(int port)
{
    Q_UNUSED( port );
    return 0;
}

std::string MSNProtocolWrapper::getOurIP(void)
{
    /*struct hostent * hn;
    char buf2[1024];

    gethostname(buf2,1024);
    hn = gethostbyname(buf2);

    return inet_ntoa( *((struct in_addr*)hn->h_addr));*/
    return "";
}

void MSNProtocolWrapper::log(int i, const char *s)
{
    Q_UNUSED( i );
    Q_UNUSED( s );
}

std::string MSNProtocolWrapper::getSecureHTTPProxy()
{
    return "";
}

void MSNProtocolWrapper::gotMessageSentACK(MSN::SwitchboardServerConnection * conn, int trID)
{
    std::cout << "Message " << trID << " delivered" << std::endl;
}

void MSNProtocolWrapper::askFileTransfer(MSN::SwitchboardServerConnection * conn, MSN::fileTransferInvite ft)
{

}

void MSNProtocolWrapper::addedContactToGroup(MSN::NotificationServerConnection * conn, bool added, std::string groupId, std::string contactId)
{
    if(added)
	printf("User Id (%s) added to group Id (%s)\n", contactId.c_str(),groupId.c_str());
    else
	printf("User Id (%s) NOT added to group Id (%s)\n", contactId.c_str(),groupId.c_str());
}

void MSNProtocolWrapper::removedContactFromGroup(MSN::NotificationServerConnection * conn, bool removed, std::string groupId, std::string contactId)
{
    if(removed)
	printf("User Id (%s) removed from group Id (%s)\n", contactId.c_str(),groupId.c_str());
    else
	printf("User Id (%s) NOT removed from group Id (%s)\n", contactId.c_str(),groupId.c_str());
}

void MSNProtocolWrapper::addedContactToAddressBook(MSN::NotificationServerConnection * conn, bool added, std::string passport, std::string displayName, std::string guid)
{
    if(added)
	printf("User (%s - %s) added to AddressBook. Guid (%s)\n", passport.c_str(),displayName.c_str(), guid.c_str());
    else
	printf("User (%s - %s) NOT added to AddressBook.\n", passport.c_str(),displayName.c_str());
}

void MSNProtocolWrapper::removedContactFromAddressBook(MSN::NotificationServerConnection * conn, bool removed, std::string contactId, std::string passport)
{
    if(removed)
	printf("User %s removed from AddressBook. Guid (%s)\n", passport.c_str(), contactId.c_str());
    else
	printf("User %s NOT removed from AddressBook. Guid (%s)\n", passport.c_str(), contactId.c_str());
}

void MSNProtocolWrapper::enabledContactOnAddressBook(MSN::NotificationServerConnection * conn, bool enabled, std::string contactId, std::string passport)
{
    // this is used to enable a contact previously disabled from msn, but not fully removed
    if(enabled)
	printf("User (%s) enabled on AddressBook. Guid (%s)\n", passport.c_str(), contactId.c_str());
    else
	printf("User (%s) NOT enabled on AddressBook. Guid (%s)\n", passport.c_str(), contactId.c_str());
}

void MSNProtocolWrapper::disabledContactOnAddressBook(MSN::NotificationServerConnection * conn, bool disabled, std::string contactId)
{
    // this is used when you have disabled this user from msn, but not deleted from hotmail
    // I suggest to delete the contact instead of disable, since I haven't tested this too much yet
    if(disabled)
	printf("User disabled on AddressBook. Guid (%s)\n", contactId.c_str());
    else
	printf("User NOT disabled on AddressBook. Guid (%s)\n", contactId.c_str());
}

size_t MSNProtocolWrapper::getDataFromSocket (void *sock, char *data, size_t size)
{
    MSNSocket *a = (MSNSocket*)sock;
    if (!a)
	return 0;
    return a->read(data, size);

}

size_t MSNProtocolWrapper::writeDataToSocket (void *sock, char *data, size_t size)
{
    MSNSocket *a = (MSNSocket*)sock;
    if (!a)
	return 0;

    return a->write(data, size);
}

void MSNProtocolWrapper::gotVoiceClipFile(MSN::SwitchboardServerConnection * conn, unsigned int sessionID, std::string file)
{
    printf("--- Voice clip file '%s'\n", file.c_str());

}

void MSNProtocolWrapper::gotEmoticonFile(MSN::SwitchboardServerConnection * conn, unsigned int sessionID, std::string alias, std::string file)
{
    printf("--- Voice emoticon file '%s' for '%s'\n", file.c_str(), alias.c_str());
}

void MSNProtocolWrapper::gotWinkFile(MSN::SwitchboardServerConnection * conn, unsigned int sessionID, std::string file)
{
    printf("--- Voice Wink file '%s'\n", file.c_str());
}




