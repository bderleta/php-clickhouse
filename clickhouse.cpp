#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_clickhouse.h"

#include "ext/standard/info.h"

extern "C" {

zend_function_entry clickhouse_functions[] = {
    PHP_FE(clickhouse_test, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry clickhouse_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_CLICKHOUSE_EXTNAME,
    clickhouse_functions,
    PHP_MINIT(clickhouse),
    PHP_MSHUTDOWN(clickhouse),
    PHP_RINIT(clickhouse),
    PHP_RSHUTDOWN(clickhouse),
    PHP_MINFO(clickhouse),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_LZSTRING_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CLICKHOUSE
ZEND_GET_MODULE(clickhouse)
#endif

PHP_RINIT_FUNCTION(clickhouse) {
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(clickhouse) {
    return SUCCESS;
}

PHP_MINIT_FUNCTION(clickhouse) {
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(clickhouse) {
    return SUCCESS;
}

PHP_MINFO_FUNCTION(clickhouse)
{
    char buf[32];

    php_info_print_table_start();
    php_info_print_table_header(2, "ClickHouse support", "enabled");
    php_info_print_table_row(2, "ClickHouse extension version", PHP_CLICKHOUSE_VERSION);
    php_info_print_table_row(2, "Build date", __DATE__);

    php_info_print_table_end();

    php_info_print_table_start();
    php_info_print_table_row(1, "ClickHouse Module realized by Bartosz Derleta.");
    php_info_print_table_end();
}

PHP_FUNCTION(clickhouse_test)
{
    RETURN_TRUE();
}

}
