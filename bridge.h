#ifndef BRIDGE_H
#define BRIDGE_H

#include <clickhouse/client.h>
#include<string>

void* chc_construct(char* host = NULL, char* username = NULL, char* password = NULL, char* default_database = NULL, long port = 9000);
void chc_destruct(void* instance);

#endif /* BRIDGE_H */

