#ifndef MOWER_H
#define MOWER_H

#include <string>

class Mower
{
public:
    Mower(std::string username, std::string password);

    enum Status {LoggedIn, LoggedOut, PendingLogIn, PendingLogOut, NotOnLAWN, Error};

    Status getStatus();
    bool login();
    bool logout();

private:
	std::string _username;
	std::string _password;

	static size_t write_function(void *ptr, size_t size, size_t nmemb, void *s);
};

#endif // MOWER_H
