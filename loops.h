#ifndef LOOPS_H
#define LOOPS_H
	
#include "config.h"

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
					string s( colCast->At(row) ); \
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
					string s( colCast->At(row) ); \
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
#define LOOP_AS_NULL for (size_t row = 0; row < rowCount; ++row) { \
					add_next_index_null(&rows[row]); \
				} \
				break
#endif

#endif /* LOOPS_H */

