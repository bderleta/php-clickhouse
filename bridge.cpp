#include "bridge.h"
#include "php.h"

using namespace std;
using namespace clickhouse;

void* chc_construct(char* host, char* username, char* password, char* default_database, long port) {
	try {
		ClientOptions opts;
		if (host) 
			opts.SetHost(string(host));
		if (username)
			opts.SetUser(string(username));
		if (password)
			opts.SetPassword(string(password));
		if (default_database)
			opts.SetDefaultDatabase(string(default_database));
		if (port)
			opts.SetPort(port);
		Client* client = new Client(opts);
		return (void*)client;
	} catch (const std::system_error& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
		return;
	}
}

void chc_destruct(void* instance) {
	Client* client = (Client*)instance;
	delete client;
}