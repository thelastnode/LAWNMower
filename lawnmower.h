#ifndef LAWNMOWER_H
#define LAWNMOWER_H

#include <string>

#include <QObject>
#include <QSystemTrayIcon>
#include <QAction>

#include "dialog.h"
#include "mower.h"

class LAWNMower : public QObject
{
	Q_OBJECT

public:
	LAWNMower();
	virtual ~LAWNMower();

public slots:
	void loadConfig(std::string username, std::string password, bool remember);
	void showDialog();
	void toggleMower(bool enabled);
	void logout();

private slots:
	void mow();

private:
	static const int REFRESH = 3000;
	static const int ERROR_REFRESH = 10000;
	bool _isEnabled;
	QSystemTrayIcon *_tray;
	QAction *_aToggle;
	Dialog *_dialog;
	Mower *_mower;

	enum Icon { Disabled, Error, LoggedIn, LoggedOut, Pending };

	bool readConfig();
	void writeConfig(std::string username, std::string password);
	void enableSystemTray();
	void setTrayIcon(Icon icon);
};

#endif // LAWNMOWER_H
