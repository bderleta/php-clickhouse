#ifndef LOOPS_H
#define LOOPS_H
	
#include "config.h"
#include <algorithm>

using Int128 = __int128;

/**
 * Convert Int128 to char*
 * Maximal value of Int128 is 39 decimal digits long. Including optional sign, decimal separator and null character, effective buffer length is 42.
 * With safety margin, this function expects a buffer at least 48 characters long. 
 * @param value
 * @param scale
 * @param buffer
 * @return 
 */
char* dec128_to_pchar(Int128 value, size_t scale, char* buffer) {
	bool sign = (value < 0);
	char* s = &buffer[48];
	size_t w = 0;
	*(--s) = 0;
	if (sign)
		value = -value;
	while (value) {
		*(--s) = (value % 10) + '0';
		if ((++w) == scale)
			*(--s) = '.';
		value /= 10;
	}
	while (w < scale) {
		*(--s) = '0';
		if ((++w) == scale)
			*(--s) = '.';
	}
	if (w == scale)
		*(--s) = '0';
	if (sign)
		*(--s) = '-';
	return s;
}

char* int128_to_pchar(Int128 value, char* buffer) {
	bool sign = (value < 0);
	char* s = &buffer[48];
	*(--s) = 0;
	if (sign)
		value = -value;
	while (value) {
		*(--s) = (value % 10) + '0';
		value /= 10;
	}
	if (sign)
		*(--s) = '-';
	return s;
}

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
#define LOOP_AS_INT128 for (size_t row = 0; row < rowCount; ++row) { \
					char* s = int128_to_pchar(colCast->At(row), int128buf); \
					add_assoc_string(&rows[row], colName, s); \
				} \
				break
#define LOOP_AS_DEC128(scale) for (size_t row = 0; row < rowCount; ++row) { \
					char* s = dec128_to_pchar(colCast->At(row), scale, int128buf); \
					add_assoc_string(&rows[row], colName, s); \
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
#define LOOP_NULLABLE_AS_INT128 for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else { \
					char* s = int128_to_pchar(colCast->At(row), int128buf); \
					add_assoc_string(&rows[row], colName, s); \
				} \
				} \
				break
#define LOOP_NULLABLE_AS_DEC128(scale) for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_assoc_null(&rows[row], colName); \
				else { \
					char* s = dec128_to_pchar(colCast->At(row), scale, int128buf); \
					add_assoc_string(&rows[row], colName, s); \
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
#ifndef OVEROPTIMIZATION_INSANE
#define LOOP_AS_INT128 for (size_t row = 0; row < rowCount; ++row) { \
					char* s = int128_to_pchar(colCast->At(row), scale, int128buf); \
					add_next_index_string(&rows[row], s); \
				} \
				break
#define LOOP_AS_DEC128(scale) for (size_t row = 0; row < rowCount; ++row) { \
					char* s = dec128_to_pchar(colCast->At(row), scale, int128buf); \
					add_next_index_string(&rows[row], s); \
				} \
				break
#else
#define LOOP_AS_INT128 for (size_t row = 0; row < rowCount; ++row) { \
					add_next_index_double(&rows[row], colCast->At(row)); \
				} \
				break
#define LOOP_AS_DEC128(scale) for (size_t row = 0; row < rowCount; ++row) { \
					add_next_index_double(&rows[row], colCast->At(row) / pow(10, scale)); \
				} \
				break
#endif
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
#ifndef OVEROPTIMIZATION_INSANE
#define LOOP_NULLABLE_AS_INT128 for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_next_index_null(&rows[row]); \
				else { \
					char* s = int128_to_pchar(colCast->At(row), int128buf); \
					add_next_index_string(&rows[row], s); \
				} \
				} \
				break
#define LOOP_NULLABLE_AS_DEC128(scale) for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_next_index_null(&rows[row]); \
				else { \
					char* s = dec128_to_pchar(colCast->At(row), scale, int128buf); \
					add_next_index_string(&rows[row], s); \
				} \
				} \
				break
#else
#define LOOP_NULLABLE_AS_INT128 for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_next_index_null(&rows[row]); \
				else { \
					add_next_index_double(&rows[row], colCast->At(row)); \
				} \
				} \
				break
#define LOOP_NULLABLE_AS_DEC128(scale) for (size_t row = 0; row < rowCount; ++row) { \
				if (outerColCast->IsNull(row)) \
					add_next_index_null(&rows[row]); \
				else { \
					add_next_index_double(&rows[row], colCast->At(row) / pow(10, scale)); \
				} \
				} \
				break
#endif
#define LOOP_AS_NULL for (size_t row = 0; row < rowCount; ++row) { \
					add_next_index_null(&rows[row]); \
				} \
				break
#endif

#endif /* LOOPS_H */

