#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_clickhouse.h"
#include <clickhouse/client.h>

#include "ext/standard/info.h"

extern "C" {

ZEND_BEGIN_ARG_INFO_EX(arginfo___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, host)
	ZEND_ARG_INFO(0, username)
	ZEND_ARG_INFO(0, password)
	ZEND_ARG_INFO(0, schema)
	ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO()	
	
zend_class_entry *clickhouse_ce;	
static zend_object_handlers clickhouse_object_handlers;

typedef struct _clickhouse_object {
    Client* client;
    long connected;
} clickhouse_object;

zend_function_entry clickhouse_functions[] = {
    PHP_ME(ClickHouse, __construct, arginfo___construct, ZEND_ACC_PUBLIC)
    PHP_FE_END
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
    PHP_CLICKHOUSE_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_CLICKHOUSE
ZEND_GET_MODULE(clickhouse)
#endif

static void clickhouse_free_object_storage_handler(clickhouse_object *intern TSRMLS_DC)
{
    efree(intern);
}

zend_object_value clickhouse_create_object_handler(zend_class_entry *class_type TSRMLS_DC)
{
    zend_object_value retval;
    clickhouse_object *intern = emalloc(sizeof(clickhouse_object));
    memset(intern, 0, sizeof(clickhouse_object));
    retval.handle = zend_objects_store_put(
        intern,
        (zend_objects_store_dtor_t) zend_objects_destroy_object,
        (zend_objects_free_object_storage_t) clickhouse_free_object_storage_handler,
        NULL TSRMLS_CC
    );
    retval.handlers = &clickhouse_object_handlers;
    return retval;
}

PHP_RINIT_FUNCTION(clickhouse) 
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(clickhouse) 
{
    return SUCCESS;
}

PHP_MINIT_FUNCTION(clickhouse) 
{
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "ClickHouse", clickhouse_functions);
    clickhouse_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(clickhouse)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(clickhouse)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "ClickHouse support", "enabled");
    php_info_print_table_row(2, "ClickHouse extension version", PHP_CLICKHOUSE_VERSION);
    php_info_print_table_row(2, "Build date", __DATE__);

    php_info_print_table_end();

    php_info_print_table_start();
    php_info_print_table_row(1, "ClickHouse Module realized by Bartosz Derleta.");
    php_info_print_table_end();
}

PHP_METHOD(ClickHouse, __construct) 
{

}

}
