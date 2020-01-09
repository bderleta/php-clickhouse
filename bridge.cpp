#include "bridge.h"
#include "loops.h"

using namespace std;
using namespace clickhouse;

void* chc_construct(char* host, char* username, char* password, char* default_database, long port, bool compression) {
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
		if (compression)
			opts.SetCompressionMethod(CompressionMethod::LZ4);
		Client* client = new Client(opts);
		return (void*)client;
	} catch (const std::system_error& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	} catch (const clickhouse::ServerException& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	}
}

void chc_destruct(void* instance) {
	Client* client = (Client*)instance;
	delete client;
}

void chc_execute(void* instance, char* query) {
	Client* client = (Client*)instance;
	try {
		client->Execute(string(query));
	} catch (const std::system_error& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	} catch (const clickhouse::ServerException& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	}	
}

void chc_ping(void* instance) {
	Client* client = (Client*)instance;
	try {
		client->Ping();
	} catch (const std::system_error& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	} catch (const clickhouse::ServerException& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	}
}

void chc_reset_connection(void* instance) {
	Client* client = (Client*)instance;
	try {
		client->ResetConnection();
	} catch (const std::system_error& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	} catch (const clickhouse::ServerException& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	}
}

size_t chc_select(void* instance, char* query, zend_fcall_info* fci, zend_fcall_info_cache* fci_cache) {
	Client* client = (Client*)instance;
	try {
		size_t total = 0;
		auto onBlock = [&fci, &fci_cache, &total] (const Block& dblock) -> bool
		{
			zval result, block;
			int ret;
			size_t rowCount = dblock.GetRowCount(), colCount = dblock.GetColumnCount();
			if (rowCount == 0)
				return true;
			total += rowCount;
			/* Build an array from resulting block */
			array_init_size(&block, rowCount);
			zval rows[rowCount];
			for (size_t i = 0; i < rowCount; ++i) {
				array_init_size(&rows[i], colCount);
				add_next_index_zval(&block, &rows[i]);
			}
			
			/* Iterate over columns */
			for (size_t col = 0; col < colCount; ++col) {
#ifndef OVEROPTIMIZATION
				const char* colName = dblock.GetColumnName(col).c_str();
#endif 
						
				/**
				 * TODO: Support for DATE / DATETIME ?
				 */
				switch (dblock[col]->Type()->GetCode()) {
					case Type::Code::Int8:
					{
						auto colCast = dblock[col]->As<ColumnInt8>();
						LOOP_AS_LONG;
					}
					case Type::Code::UInt8:
					{
						auto colCast = dblock[col]->As<ColumnUInt8>();
						LOOP_AS_LONG;
					}
					case Type::Code::Int16:
					{
						auto colCast = dblock[col]->As<ColumnInt16>();
						LOOP_AS_LONG;
					}
					case Type::Code::UInt16:
					{
						auto colCast = dblock[col]->As<ColumnUInt16>();
						LOOP_AS_LONG;
					}
					case Type::Code::Int32:
					{
						auto colCast = dblock[col]->As<ColumnInt32>();
						LOOP_AS_LONG;
					}
					case Type::Code::UInt32:
					{
						auto colCast = dblock[col]->As<ColumnUInt32>();
						LOOP_AS_LONG;
					}
					case Type::Code::Int64:
					{
						auto colCast = dblock[col]->As<ColumnInt64>();
						LOOP_AS_LONG;
					}
					case Type::Code::UInt64:
					{
						auto colCast = dblock[col]->As<ColumnUInt64>();
						LOOP_AS_LONG;
					}
					case Type::Code::Float32:
					{
						auto colCast = dblock[col]->As<ColumnFloat32>();
						LOOP_AS_LONG;
					}
					case Type::Code::Float64:
					{
						auto colCast = dblock[col]->As<ColumnFloat64>();
						LOOP_AS_LONG;
					}
					case Type::Code::Enum8:
					{
						auto colCast = dblock[col]->As<ColumnEnum8>();
						LOOP_AS_ENUM;
					}
					case Type::Code::Enum16:
					{
						auto colCast = dblock[col]->As<ColumnEnum16>();
						LOOP_AS_ENUM;
					}
					case Type::Code::String:
					{
						auto colCast = dblock[col]->As<ColumnString>();
						LOOP_AS_STRING;
					}
					case Type::Code::FixedString:
					{
						auto colCast = dblock[col]->As<ColumnFixedString>();
						LOOP_AS_STRING;
					}
					case Type::Code::Nullable:
					{
						auto outerColCast = dblock[col]->As<ColumnNullable>();
						switch (outerColCast->Type()->As<NullableType>()->GetNestedType()->GetCode()) {
							case Type::Code::Int8:
							{
								auto colCast = outerColCast->Nested()->As<ColumnInt8>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::UInt8:
							{
								auto colCast = outerColCast->Nested()->As<ColumnUInt8>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::Int16:
							{
								auto colCast = outerColCast->Nested()->As<ColumnInt16>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::UInt16:
							{
								auto colCast = outerColCast->Nested()->As<ColumnUInt16>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::Int32:
							{
								auto colCast = outerColCast->Nested()->As<ColumnInt32>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::UInt32:
							{
								auto colCast = outerColCast->Nested()->As<ColumnUInt32>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::Int64:
							{
								auto colCast = outerColCast->Nested()->As<ColumnInt64>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::UInt64:
							{
								auto colCast = outerColCast->Nested()->As<ColumnUInt64>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::Float32:
							{
								auto colCast = outerColCast->Nested()->As<ColumnFloat32>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::Float64:
							{
								auto colCast = outerColCast->Nested()->As<ColumnFloat64>();
								LOOP_NULLABLE_AS_LONG;
							}
							case Type::Code::Enum8:
							{
								auto colCast = outerColCast->Nested()->As<ColumnEnum8>();
								LOOP_NULLABLE_AS_ENUM;
							}
							case Type::Code::Enum16:
							{
								auto colCast = outerColCast->Nested()->As<ColumnEnum16>();
								LOOP_NULLABLE_AS_ENUM;
							}
							case Type::Code::String:
							{
								auto colCast = outerColCast->Nested()->As<ColumnString>();
								LOOP_NULLABLE_AS_STRING;
							}
							case Type::Code::FixedString:
							{
								auto colCast = outerColCast->Nested()->As<ColumnFixedString>();
								LOOP_NULLABLE_AS_STRING;
							}
							default:
							{
								LOOP_AS_NULL;
							}
						}
					}
					default:
					{
						LOOP_AS_NULL;
					}
				}
			}	
			/* Send to callback */
			fci->retval = &result;
			fci->param_count = 1;
			fci->params = &block;
			fci->no_separation = 0;
			ret = zend_call_function(fci, fci_cache);
			zval_ptr_dtor(&block);
			/* If not a boolean is returned, assume continue */
			if ((Z_TYPE(result) != IS_TRUE) && (Z_TYPE(result) != IS_FALSE)) 
				return true;
			return (Z_TYPE(result) == IS_TRUE);
		};
		client->SelectCancelable(string(query), onBlock);
		return total;
	} catch (const std::system_error& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	} catch (const clickhouse::ServerException& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	}
}