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

size_t chc_select(void* instance, char* query, zend_fcall_info* fci, zend_fcall_info_cache* fci_cache) {
	Client* client = (Client*)instance;
	try {
		size_t total = 0;
		client->Select(string(query), [&fci, &fci_cache, &total] (const Block& dblock)
		{
			zval block, result;
			int ret;
			size_t rowCount = dblock.GetRowCount(), colCount = dblock.GetColumnCount();
			if (rowCount == 0)
				return;
			total += rowCount;
			/* Build an array from resulting block */
			array_init_size(&block, rowCount);
			zval* rowCache[rowCount];
			for (size_t i = 0; i < rowCount; ++i) {
				zval row;
				array_init_size(&row, colCount);
				add_next_index_zval(&block, &row);
				rowCache[i] = &row;
			}
			
			/* Iterate over columns */
			for (size_t col = 0; col < colCount; ++col) {
				switch (dblock[col]->Type()->GetCode()) {
					case Type::Code::Int8:
						for (auto it = dblock[col]->As<ColumnInt8>->begin(); it != dblock[col]->As<ColumnInt8>->end(); ++it) {
							
						}
						break;
				}
			}
			
#ifdef DUPA			
				for (size_t j = 0; j < colCount; ++j) {
					switch (codes[j]) {
/* TODO: Significantly optimize iteration over block */						
#ifdef ASSOC 
						case Type::Code::Int8:
							add_assoc_long(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnInt8>()->At(i)); break;
						case Type::Code::UInt8:
							add_assoc_long(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnUInt8>()->At(i)); break;
						case Type::Code::Int16:
							add_assoc_long(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnInt16>()->At(i)); break;
						case Type::Code::UInt16:
							add_assoc_long(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnUInt16>()->At(i)); break;
						case Type::Code::Int32:
							add_assoc_long(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnInt32>()->At(i)); break;
						case Type::Code::UInt32:
							add_assoc_long(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnUInt32>()->At(i)); break;
						case Type::Code::Int64:
							add_assoc_long(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnInt64>()->At(i)); break;
						case Type::Code::UInt64:
							add_assoc_long(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnUInt64>()->At(i)); break;
						case Type::Code::Float32:
							add_assoc_double(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnFloat32>()->At(i)); break;
						case Type::Code::Float64: 
							add_assoc_double(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnFloat64>()->At(i)); break;
						case Type::Code::String:
						case Type::Code::FixedString:
							add_assoc_stringl(&row, dblock.GetColumnName(j).c_str(), dblock[j]->As<ColumnString>()->At(i).c_str(), dblock[j]->As<ColumnString>()->At(i).length()); break;
						default:
							add_assoc_null(&row, dblock.GetColumnName(j).c_str());
#else
						case Type::Code::Int8:
							add_next_index_long(&row, dblock[j]->As<ColumnInt8>()->At(i)); break;
						case Type::Code::UInt8:
							add_next_index_long(&row, dblock[j]->As<ColumnUInt8>()->At(i)); break;
						case Type::Code::Int16:
							add_next_index_long(&row, dblock[j]->As<ColumnInt16>()->At(i)); break;
						case Type::Code::UInt16:
							add_next_index_long(&row, dblock[j]->As<ColumnUInt16>()->At(i)); break;
						case Type::Code::Int32:
							add_next_index_long(&row, dblock[j]->As<ColumnInt32>()->At(i)); break;
						case Type::Code::UInt32:
							add_next_index_long(&row, dblock[j]->As<ColumnUInt32>()->At(i)); break;
						case Type::Code::Int64:
							add_next_index_long(&row, dblock[j]->As<ColumnInt64>()->At(i)); break;
						case Type::Code::UInt64:
							add_next_index_long(&row, dblock[j]->As<ColumnUInt64>()->At(i)); break;
						case Type::Code::Float32:
							add_next_index_double(&row, dblock[j]->As<ColumnFloat32>()->At(i)); break;
						case Type::Code::Float64: 
							add_next_index_double(&row, dblock[j]->As<ColumnFloat64>()->At(i)); break;
						case Type::Code::String:
						case Type::Code::FixedString:
							add_next_index_stringl(&row, dblock[j]->As<ColumnString>()->At(i).c_str(), dblock[j]->As<ColumnString>()->At(i).length()); break;
						default:
							add_next_index_null(&row);
#endif
					}
				}
				add_next_index_zval(&block, &row);
			}
#endif		
		
			/* Send to callback */
			fci->retval = &result;
			fci->param_count = 1;
			fci->params = &block;
			fci->no_separation = 0;
			ret = zend_call_function(fci, fci_cache);
			i_zval_ptr_dtor(&block);
		});
		return total;
	} catch (const std::system_error& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	}
}