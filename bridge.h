#ifndef BRIDGE_H
#define BRIDGE_H

#include <clickhouse/client.h>
#include <string>
#include <system_error>

#include "php.h"
#include "zend_types.h"
#include "Zend/zend_exceptions.h"

void* chc_construct(char* host = NULL, char* username = NULL, char* password = NULL, char* default_database = NULL, long port = 9000, bool compression = true);
void chc_destruct(void* instance);
size_t chc_select(void* instance, char* query, zend_fcall_info* fci, zend_fcall_info_cache* fci_cache);
void chc_execute(void* instance, char* query);
void chc_ping(void* instance);

#endif /* BRIDGE_H */

