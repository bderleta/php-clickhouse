#ifndef LOOPS_H
#define LOOPS_H
	
#include "config.h"
#include <algorithm>

using Int128 = __int128;

auto int128_to_string = [](Int128 value, size_t scale) {
	php_printf("Converting value with scale %lu !!!\r\n", scale);
	std::string result;
	const bool sign = value >= 0;

	if (!sign) {
		value = -value;
	}

	size_t w = 0;
	while (value) {
		result += static_cast<char>(value % 10) + '0';
		if ((w++) == scale)
			result += ".";
		value /= 10;
	}

	if (result.empty()) {
		result = "0";
	} else if (!sign) {
		result.push_back('-');
	}

	std::reverse(result.begin(), result.end());

	return result;
};

#ifndef OVEROPTIMIZATION
#define LOOP_AS_LONG for (size_t row = 0; row < rowCount; ++row) { \
					add_assoc_long(&rows[row], colName, colCast->At(row)); \
				} \
				break
#define LOOP_AS_DOUBLE for (size_t row = 0; row < rowCount; ++row) { \
					add_assoc_double(&rows[row], colName, colCast->At(row)); \
				} \
				break			
#define LOOP_AS_ENUM for (size_t row = 0; row < rowCount; ++row) { \
					add_assoc_stringl(&rows[row], colName, colCast->NameAt(row).c_str(), colCast->NameAt(row).length()); \
				} \
				break
#define LOOP_AS_STRING for (size_t row = 0; row < rowCount; ++row) { \
					add_assoc_stringl(&rows[row], colName, colCast->At(row).c_str(), colCast->At(row).length()); \
				} \
				break
#define LOOP_AS_CAST_STRING for (size_t row = 0; row < rowCount; ++row) { \
					string s = to_string(colCast->At(row)); \
					add_assoc_stringl(&rows[row], colName, s.c_str(), s.length()); \
				} \
				break
#define LOOP_AS_INT128(scale) for (size_t row = 0; row < rowCount; ++row) { \
					string s = int128_to_string(colCast->At(row), scale); \
					add_assoc_stringl(&rows[row], colName, s.c_str(), s.length()); \
				} \
				break
#define LOOP_NULLABLE_AS_LONG for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else \
					add_assoc_long(&rows[row], colName, colCast->At(row)); \
				} \
				break
#define LOOP_NULLABLE_AS_DOUBLE for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else \
					add_assoc_double(&rows[row], colName, colCast->At(row)); \
				} \
				break
#define LOOP_NULLABLE_AS_ENUM for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else \
					add_assoc_stringl(&rows[row], colName, colCast->NameAt(row).c_str(), colCast->NameAt(row).length()); \
				} \
				break
#define LOOP_NULLABLE_AS_STRING for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else \
					add_assoc_stringl(&rows[row], colName, colCast->At(row).c_str(), colCast->At(row).length()); \
				} \
				break
#define LOOP_NULLABLE_AS_CAST_STRING for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else { \
					string s = to_string(colCast->At(row)); \
					add_assoc_stringl(&rows[row], colName, s.c_str(), s.length()); \
				} \
				} \
				break
#define LOOP_NULLABLE_AS_INT128(scale) for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else { \
					string s = int128_to_string(colCast->At(row), scale); \
					add_assoc_stringl(&rows[row], colName, s.c_str(), s.length()); \
				} \
				} \
				break
#define LOOP_AS_NULL for (size_t row = 0; row < rowCount; ++row) { \
					add_assoc_null(&rows[row], colName); \
				} \
				break
#else
#define LOOP_AS_LONG for (size_t row = 0; row < rowCount; ++row) { \
					add_next_index_long(&rows[row], colCast->At(row)); \
				} \
				break
#define LOOP_AS_DOUBLE for (size_t row = 0; row < rowCount; ++row) { \
					add_next_index_double(&rows[row], colCast->At(row)); \
				} \
				break
#define LOOP_AS_ENUM for (size_t row = 0; row < rowCount; ++row) { \
					add_next_index_stringl(&rows[row], colCast->NameAt(row).c_str(), colCast->NameAt(row).length()); \
				} \
				break
#define LOOP_AS_STRING for (size_t row = 0; row < rowCount; ++row) { \
					add_next_index_stringl(&rows[row], colCast->At(row).c_str(), colCast->At(row).length()); \
				} \
				break
#define LOOP_AS_CAST_STRING for (size_t row = 0; row < rowCount; ++row) { \
					string s = to_string(colCast->At(row)); \
					add_next_index_stringl(&rows[row], s.c_str(), s.length()); \
				} \
				break
#define LOOP_AS_INT128(scale) for (size_t row = 0; row < rowCount; ++row) { \
					string s = int128_to_string(colCast->At(row), scale); \
					add_next_index_stringl(&rows[row], s.c_str(), s.length()); \
				} \
				break
#define LOOP_NULLABLE_AS_LONG for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_next_index_null(&rows[row]); \
				else \
					add_next_index_long(&rows[row], colCast->At(row)); \
				} \
				break
#define LOOP_NULLABLE_AS_DOUBLE for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_next_index_null(&rows[row]); \
				else \
					add_next_index_double(&rows[row], colCast->At(row)); \
				} \
				break
#define LOOP_NULLABLE_AS_ENUM for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_next_index_null(&rows[row]); \
				else \
					add_next_index_stringl(&rows[row], colCast->NameAt(row).c_str(), colCast->NameAt(row).length()); \
				} \
				break
#define LOOP_NULLABLE_AS_STRING for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_next_index_null(&rows[row]); \
				else \
					add_next_index_stringl(&rows[row], colCast->At(row).c_str(), colCast->At(row).length()); \
				} \
				break
#define LOOP_NULLABLE_AS_CAST_STRING for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else { \
					string s = to_string(colCast->At(row)); \
					add_next_index_stringl(&rows[row], s.c_str(), s.length()); \
				} \
				} \
				break
#define LOOP_NULLABLE_AS_INT128(scale) for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else { \
					string s = int128_to_string(colCast->At(row), scale); \
					add_next_index_stringl(&rows[row], s.c_str(), s.length()); \
				} \
				} \
				break
#define LOOP_AS_NULL for (size_t row = 0; row < rowCount; ++row) { \
					add_next_index_null(&rows[row]); \
				} \
				break
#endif

#endif /* LOOPS_H */

