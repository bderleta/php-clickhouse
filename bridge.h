#ifndef BRIDGE_H
#define BRIDGE_H

#include <clickhouse/client.h>
#include <string>
#include <system_error>

#include "php.h"
#include "Zend/zend_exceptions.h"

void* chc_construct(char* host = NULL, char* username = NULL, char* password = NULL, char* default_database = NULL, long port = 9000);
void chc_destruct(void* instance);
void chc_select(void* instance, char* query, zend_fcall_info* fci, zend_fcall_info_cache* fci_cache);

#endif /* BRIDGE_H */

