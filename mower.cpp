// TODO delete this
#include <iostream>

#include <curl/curl.h>

#include "mower.h"

Mower::Mower(std::string username, std::string password) :
		_username(username), _password(password)
{
}

Mower::Status Mower::getStatus()
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		std::string s;
		curl_easy_setopt(curl, CURLOPT_URL, "https://auth.lawn.gatech.edu/login_status.php?output=text");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);

#ifdef SKIP_PEER_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERFICATION
	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);

		if (res != 0) {
			return Mower::Error;
		}

		s = s.substr(0, s.find(','));

		if (s == "logged_in") {
			return Mower::LoggedIn;
		} else if (s == "not_authenticated") {
			return Mower::LoggedOut;
		} else if (s == "pending_login") {
			return Mower::PendingLogIn;
		} else if (s == "pending_logout") {
			return Mower::PendingLogOut;
		} else if (s == "not_on_LAWN") {
			return Mower::NotOnLAWN;
		}

		std::cerr << "LAWN ERROR: " << s;
	}

	return Mower::Error;
}

bool Mower::login()
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		std::string s;
		curl_easy_setopt(curl, CURLOPT_URL, "https://auth.lawn.gatech.edu/index.php");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);

		char *user = curl_easy_escape(curl, _username.c_str(), 0);
		char *pass = curl_easy_escape(curl, _password.c_str(), 0);
		std::string post;
		post.append("username=");
		post.append(user);
		post.append("&password=");
		post.append(pass);
		post.append("&output=text");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());

#ifdef SKIP_PEER_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERFICATION
	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);

		if (res == 0) {
			return true;
		}
	}

	return false;
}

bool Mower::logout()
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		std::string s;
		curl_easy_setopt(curl, CURLOPT_URL, "https://auth.lawn.gatech.edu/logout.php");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_function);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "output=text");

#ifdef SKIP_PEER_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERFICATION
	    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);

		if (res == 0) {
			return true;
		}
	}

	return false;
}

size_t Mower::write_function(void *ptr, size_t size, size_t nmemb, void *userp)
{
	std::string* s = static_cast<std::string*>(userp);
	char *c = static_cast<char*>(ptr);
	for (unsigned int i = 0; i < size * nmemb - 1; i += size) {
		*s += c[i];
	}
	return size*nmemb;
}
