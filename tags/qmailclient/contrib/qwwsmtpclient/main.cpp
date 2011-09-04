#include <QApplication>
#include <QStringList>
#include "qwwsmtpclient.h"

int main(int argc, char **argv){
  QApplication app(argc, argv);
  QwwSmtpClient client;
  app.connect(&client, SIGNAL(sslErrors(const QList<QSslError> &)), &client, SLOT(ignoreSslErrors()));
  client.connectToHost("mail.example.com");
  client.startTls();
  client.authenticate("login", "password", QwwSmtpClient::AuthLogin);
  client.sendMail("from@example.com", "to@example.com", "From: from@example.com\r\nTo: to@example.com\r\nSubject: QwwSmtpClient\r\n\r\nMail content");
  client.disconnectFromHost();
  app.connect(&client, SIGNAL(disconnected()), &app, SLOT(quit()));
  return app.exec();
}
