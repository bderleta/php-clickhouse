#include "bridge.h"

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
	try {
		client->Select(string(query), [&fci, &fci_cache] (const Block& dblock)
		{
			zval block, result;
			int ret;
			/* Build an array from resulting block */
			array_init(&block);
			for (size_t i = 0; i < dblock.GetRowCount(); ++i) {
				zval row;
				array_init(&row);
				for (size_t j = 0; j < dblock.GetColumnCount(); ++j) {
					switch (dblock[j]->Type()->GetCode()) {
						case Type::Code::Int8:
						case Type::Code::UInt8:
						case Type::Code::Int16:
						case Type::Code::UInt16:
						case Type::Code::Int32:
						case Type::Code::UInt32:
						case Type::Code::Int64:
						case Type::Code::UInt64:
							add_next_index_long(&row, dblock[j]->As<ColumnInt64>()->At(i));
						case Type::Code::Float32:
						case Type::Code::Float64:
							add_next_index_double(&row, dblock[j]->As<ColumnFloat64>()->At(i));
						case Type::Code::String:
						case Type::Code::FixedString:
							add_next_index_stringl(&row, dblock[j]->As<ColumnString>()->At(i).c_str(), dblock[j]->As<ColumnString>()->At(i).length());
							break;
						default:
							add_next_index_null(&row);
					}
				}
				add_next_index_zval(&block, &row);
			}
			/* Send to callback */
			fci->retval = &result;
			fci->param_count = 1;
			fci->params = &block;
			fci->no_separation = 0;
			ret = zend_call_function(fci, fci_cache);
			i_zval_ptr_dtor(&block);
		});
	} catch (const std::system_error& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	}
}