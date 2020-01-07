#ifndef BRIDGE_H
#define BRIDGE_H

#include <clickhouse/client.h>
#include<string>

void* construct(char* host = NULL, char* username = NULL, char* password = NULL, char* default_database = NULL, long port = 9000);

#endif /* BRIDGE_H */

