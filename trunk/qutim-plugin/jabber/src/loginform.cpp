#include "loginform.h"


ClientThread::ClientThread(LoginForm *login, const QString &server, const QString &username, const QString &password)
	: m_login_form(login), m_username(username), m_password(password)
{
	r_client = new Client(utils::toStd(server));
	r_client->logInstance().registerLogHandler(LogLevelDebug,LogAreaXmlIncoming|LogAreaXmlOutgoing,this);
	r_client->disableRoster();
	r_client->registerConnectionListener(this);
	r_connection = new jConnection(r_client, "", server);
	r_connection->loadSettings();
	r_client->setConnectionImpl(r_connection);
	r_client->connect(false);
	qDebug() << "connecting for registration";
}


void ClientThread::handleLog (LogLevel, LogArea area, const std::string&message)
{
	bool in;
	if(area == LogAreaXmlIncoming)
		in = true;
	else if(area == LogAreaXmlOutgoing)
		in = false;
	QString xml = utils::fromStd(message);
	qDebug() << (in?"> ":"< ") << xml;
}


ClientThread::~ClientThread()
{
	r_manager->removeRegistrationHandler();
	delete r_manager;
	r_client->removeConnectionListener(this);
	delete r_client;
}

void ClientThread::onConnect ()
{
	r_manager = new Registration(r_client);
	r_manager->registerRegistrationHandler(m_login_form);
	RegistrationFields regFields;
	regFields.username = utils::toStd(m_username);
	regFields.password = utils::toStd(m_password);
	r_manager->createAccount(Registration::FieldUsername|Registration::FieldPassword, regFields);
	qDebug() << "connected";
}

LoginForm::LoginForm(QWidget *parent) : QWidget(parent)
{
	ui.setupUi(this);
	//ui.pushRegister->setVisible(false);
}

LoginForm::~LoginForm()
{
}

void LoginForm::btnRegisterClick()
{
	if (ui.jidEdit->text().count("@") != 1)
	{
		QMessageBox::warning(this, tr("Registration"), tr("You must enter a valid jid"));
		return;
	}
	if (ui.passowrdEdit->text().isEmpty())
	{
		QMessageBox::warning(this, tr("Registration"), tr("You must enter a password"));
		return;
	}
	//ui.labelResult->setText(tr("Registration..."));
	movie = new QMovie(ui.labelResult);
	movie->setFileName(SystemsCity::IconManager()->getIconPath("loading"));
	movie->start();
	ui.labelResult->setMovie(movie);
	cltrd = new ClientThread(this,utils::fromStd(JID(utils::toStd(ui.jidEdit->text())).server()),utils::fromStd(JID(utils::toStd(ui.jidEdit->text())).username()),ui.passowrdEdit->text());
}

void LoginForm::handleRegistrationResult(const JID &from, RegistrationResult regResult)
{
	delete movie;
	if (regResult == RegistrationSuccess)
		ui.labelResult->setText(tr("<font color='green'>%1</font>").arg("Registration successful completed"));
	else if (regResult == RegistrationConflict)
		ui.labelResult->setText(tr("<font color='red'>%1</font>").arg("Username already registered"));
	else
		ui.labelResult->setText(tr("<font color='red'>%1</font>").arg("Registration failed"));
	qDebug() << regResult;
	cltrd->disconnectFromServer();
	QTimer::singleShot(0, this, SLOT(doCleanup()));
}

void LoginForm::doCleanup()
{
	delete cltrd;
}
