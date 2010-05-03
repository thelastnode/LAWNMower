#include <QTimer>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMenu>
#include <QApplication>
#include <QNetworkReply>

#include "lawnmower.h"

LAWNMower::LAWNMower()
{
	_net = new QNetworkAccessManager(this);
	connect(_net, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyReceived(QNetworkReply*)));
	_dialog = new Dialog;
	connect(_dialog, SIGNAL(accepted(std::string,std::string, bool)), this, SLOT(loadConfig(std::string,std::string, bool)));
	_mower = 0;

	enableSystemTray();
	setTrayIcon(LAWNMower::Disabled);
	_tray->show();

	if (readConfig()) {
		toggleMower(true);
	} else {
		_dialog->show();

	}
}

LAWNMower::~LAWNMower()
{
	delete _mower;
}

void LAWNMower::loadConfig(std::string username, std::string password, bool remember)
{
	if (_mower) {
		delete _mower;
	}
	_mower = new Mower(_net, username, password);
	if (remember) {
		writeConfig(username, password);
	}
	this->toggleMower(true);
}

void LAWNMower::writeConfig(std::string username, std::string password)
{
	QFile configFile(QDir::homePath() + "/.lawnmower");
	if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&configFile);
		out << username.c_str() << "\n" << password.c_str();
	}
}


bool LAWNMower::readConfig()
{
	QFile configFile(QDir::homePath() + "/.lawnmower");
	if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
	    return false;

	QTextStream in(&configFile);
	std::string username = in.readLine().toStdString();
	std::string password = in.readLine().toStdString();

	_mower = new Mower(_net, username, password);

	return true;
}

void LAWNMower::toggleMower(bool enabled)
{
	if (_mower) {
		if (enabled) {
			_isEnabled = true;
			_mower->getStatus();
		} else {
			_isEnabled = false;
			setTrayIcon(LAWNMower::Disabled);
		}
	}
	_aToggle->setChecked(_isEnabled);
}

void LAWNMower::mow(Mower::Status status)
{
	if (!_isEnabled) {
		return;
	}
	int ms = REFRESH;
	switch (status) {
		case Mower::LoggedIn:
			setTrayIcon(LAWNMower::LoggedIn);
			break;
		case Mower::LoggedOut:
			setTrayIcon(LAWNMower::LoggedOut);
			_mower->login();
			break;
		case Mower::PendingLogIn:
			setTrayIcon(LAWNMower::Pending);
			break;
		case Mower::PendingLogOut:
			setTrayIcon(LAWNMower::Pending);
			break;
		case Mower::NotOnLAWN:
			ms = ERROR_REFRESH;
			setTrayIcon(LAWNMower::Disabled);
			break;
		case Mower::Error:
			ms = ERROR_REFRESH;
			setTrayIcon(LAWNMower::Error);
			break;
	}
	QTimer::singleShot(ms, _mower, SLOT(getStatus()));
}

void LAWNMower::setTrayIcon(Icon icon)
{
	QIcon *q = 0;
	switch (icon) {
		case Disabled:
			q = new QIcon(":/images/disabled.png");
			break;
		case Error:
			q = new QIcon(":/images/error.png");
			break;
		case LoggedIn:
			q = new QIcon(":/images/logged_in.png");
			break;
		case LoggedOut:
			q = new QIcon(":/images/logged_out.png");
			break;
		case Pending:
			q = new QIcon(":/images/pending.png");
			break;
	}
	_tray->setIcon(QIcon(*q));
}

void LAWNMower::enableSystemTray()
{
	_tray = new QSystemTrayIcon;
	QMenu *menu = new QMenu;

	QAction *aConfig = menu->addAction(tr("Configure..."));
	connect(aConfig, SIGNAL(triggered()), this, SLOT(showDialog()));

	_aToggle = menu->addAction(tr("Toggle Mower"));
	_aToggle->setCheckable(true);
	_aToggle->setChecked(false);
	connect(_aToggle, SIGNAL(triggered(bool)), this, SLOT(toggleMower(bool)));

	/* // WTF THIS DOESN'T WORK FOR SOME REASON.
	QAction *aLogout = menu->addAction(tr("Log Out"));
	connect(aLogout, SIGNAL(triggered()), this, SLOT(logout()));
	*/

	QAction *aQuit = menu->addAction(tr("Quit"));
	connect(aQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

	_tray->setContextMenu(menu);
}

void LAWNMower::showDialog()
{
	_dialog->show();
}

void LAWNMower::logout()
{
	// TODO Implement...
	toggleMower(false);
	_mower->logout();
}

void LAWNMower::replyReceived(QNetworkReply *reply)
{
	if (!_mower) {
		return;
	}
	if (_mower->getState() == Mower::GettingStatus) {
		std::string s(QString(reply->readAll()).toStdString());
		mow(Mower::strToStatus(s));
        }
        reply->deleteLater();
}
