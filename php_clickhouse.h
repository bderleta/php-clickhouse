#ifndef PHP_CLICKHOUSE_H
#define PHP_CLICKHOUSE_H 1

#define PHP_CLICKHOUSE_VERSION "20.1.7"
#define PHP_CLICKHOUSE_EXTNAME "clickhouse"

extern "C" {

extern zend_module_entry clickhouse_module_entry;
#define phpext_clickhouse_ptr &clickhouse_module_entry

PHP_MINIT_FUNCTION(clickhouse);
PHP_MSHUTDOWN_FUNCTION(clickhouse);
PHP_RINIT_FUNCTION(clickhouse);
PHP_RSHUTDOWN_FUNCTION(clickhouse);
PHP_MINFO_FUNCTION(clickhouse);

PHP_METHOD(ClickHouse, __construct); 
PHP_METHOD(ClickHouse, select); 
PHP_METHOD(ClickHouse, execute); 
PHP_METHOD(ClickHouse, ping); 
PHP_METHOD(ClickHouse, resetConnection); 

}

#endif
