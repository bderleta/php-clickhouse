#include "bridge.h"
#include "php.h"
#include "Zend/zend_exceptions.h"

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
		return NULL;
	}
}

void chc_destruct(void* instance) {
	Client* client = (Client*)instance;
	delete client;
}

void chc_select(void* instance, char* query, zend_fcall_info* fci, zend_fcall_info_cache* fci_cache) {
	Client* client = (Client*)instance;
	client->Select(string(query), [] (const Block& dblock)
	{
		zval block;
		array_init(&block);
		for (size_t i = 0; i < dblock.GetRowCount(); ++i) {
			zval row;
			array_init(&row);
			for (size_t j = 0; j < dblock.GetColumnCount(); ++j) {
				switch (dblock[j]->Type()->GetCode()) {
					case Type::Code::String:
					case Type::Code::FixedString:
						add_next_index_string(&row, dblock[j]->As<ColumnString>()->At(i).c_str(), dblock[j]->As<ColumnString>()->At(i).length(), true);
						break;
					default:
						add_next_index_long(&row, 69);
				}
			}
			add_next_index_zval(&block, &row);
		}
	});
}