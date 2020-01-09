#include "bridge.h"

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
		auto onBlock = [&fci, &fci_cache, &total] (const Block& dblock)
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
				const char* colName = dblock.GetColumnName(col).c_str();
				
#define LOOP_AS_LONG for (size_t row = 0; row < rowCount; ++row) { \
					add_assoc_long(rowCache[row], colName, colCast->At(row)); \
				} \
				break
#define LOOP_AS_DOUBLE for (size_t row = 0; row < rowCount; ++row) { \
					add_assoc_double(rowCache[row], colName, colCast->At(row)); \
				} \
				break
#define LOOP_AS_ENUM for (size_t row = 0; row < rowCount; ++row) { \
					add_assoc_stringl(rowCache[row], colName, colCast->NameAt(row).c_str(), colCast->NameAt(row).length()); \
				} \
				break
#define LOOP_AS_STRING for (size_t row = 0; row < rowCount; ++row) { \
					add_assoc_stringl(rowCache[row], colName, colCast->At(row).c_str(), colCast->At(row).length()); \
				} \
				break
				
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
						for (size_t row = 0; row < rowCount; ++row) {
							if (outerColCast->IsNull(row)) {
								add_assoc_null(rowCache[row], colName);	
							} else {
								switch (outerColCast->Type()->As<NullableType>()->GetNestedType()->GetCode()) {
									case Type::Code::Int8:
									{
										auto colCast = outerColCast->Nested()->As<ColumnInt8>();
										LOOP_AS_LONG;
									}
									case Type::Code::UInt8:
									{
										auto colCast = outerColCast->Nested()->As<ColumnUInt8>();
										LOOP_AS_LONG;
									}
									case Type::Code::Int16:
									{
										auto colCast = outerColCast->Nested()->As<ColumnInt16>();
										LOOP_AS_LONG;
									}
									case Type::Code::UInt16:
									{
										auto colCast = outerColCast->Nested()->As<ColumnUInt16>();
										LOOP_AS_LONG;
									}
									case Type::Code::Int32:
									{
										auto colCast = outerColCast->Nested()->As<ColumnInt32>();
										LOOP_AS_LONG;
									}
									case Type::Code::UInt32:
									{
										auto colCast = outerColCast->Nested()->As<ColumnUInt32>();
										LOOP_AS_LONG;
									}
									case Type::Code::Int64:
									{
										auto colCast = outerColCast->Nested()->As<ColumnInt64>();
										LOOP_AS_LONG;
									}
									case Type::Code::UInt64:
									{
										auto colCast = outerColCast->Nested()->As<ColumnUInt64>();
										LOOP_AS_LONG;
									}
									case Type::Code::Float32:
									{
										auto colCast = outerColCast->Nested()->As<ColumnFloat32>();
										LOOP_AS_LONG;
									}
									case Type::Code::Float64:
									{
										auto colCast = outerColCast->Nested()->As<ColumnFloat64>();
										LOOP_AS_LONG;
									}
									case Type::Code::Enum8:
									{
										auto colCast = outerColCast->Nested()->As<ColumnEnum8>();
										LOOP_AS_ENUM;
									}
									case Type::Code::Enum16:
									{
										auto colCast = outerColCast->Nested()->As<ColumnEnum16>();
										LOOP_AS_ENUM;
									}
									case Type::Code::String:
									{
										auto colCast = outerColCast->Nested()->As<ColumnString>();
										LOOP_AS_STRING;
									}
									case Type::Code::FixedString:
									{
										auto colCast = outerColCast->Nested()->As<ColumnFixedString>();
										LOOP_AS_STRING;
									}
									default:
									{
										for (size_t row = 0; row < rowCount; ++row) {
											add_assoc_null(rowCache[row], colName);	
										}
									}
								}
							}
						}
					}
					default:
					{
						for (size_t row = 0; row < rowCount; ++row) {
							add_assoc_null(rowCache[row], colName);	
						}
					}
				}
#undef LOOP_AS_LONG
#undef LOOP_AS_DOUBLE
#undef LOOP_AS_ENUM
#undef LOOP_AS_STRING
			}	
			/* Send to callback */
			fci->retval = &result;
			fci->param_count = 1;
			fci->params = &block;
			fci->no_separation = 0;
			ret = zend_call_function(fci, fci_cache);
			i_zval_ptr_dtor(&block);
		};
		client->Select(string(query), onBlock);
		return total;
	} catch (const std::system_error& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	} catch (const clickhouse::ServerException& e) {
		zend_throw_exception_ex(zend_exception_get_default(), 1 TSRMLS_CC, e.what());
	}
}