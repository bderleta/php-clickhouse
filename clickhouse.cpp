#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_clickhouse.h"
#include "bridge.h"

#include "ext/standard/info.h"

extern "C" {

int clickhouse_obj_res_num; 
	
ZEND_BEGIN_ARG_INFO_EX(arginfo___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, host)
	ZEND_ARG_INFO(0, username)
	ZEND_ARG_INFO(0, password)
	ZEND_ARG_INFO(0, default_database)
	ZEND_ARG_INFO(0, port)
ZEND_END_ARG_INFO()	
	
zend_class_entry *clickhouse_ce;	

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

PHP_RINIT_FUNCTION(clickhouse) 
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(clickhouse) 
{
    return SUCCESS;
}

static void clickhouse_obj_res_dtor(zend_resource *rsrc)
{
    chc_destruct((void*)rsrc->ptr);
}

PHP_MINIT_FUNCTION(clickhouse) 
{
	/* Register class name */
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "ClickHouse", clickhouse_functions);
    clickhouse_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);
	zend_declare_property_null(clickhouse_ce, "connection", sizeof("connection") - 1, ZEND_ACC_PRIVATE TSRMLS_CC);
	/* Register connection object as resource */
	clickhouse_obj_res_num = zend_register_list_destructors_ex(clickhouse_obj_res_dtor, NULL, "clickhouse_client", module_number);
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
	void* ch_object = chc_construct("10.8.86.165", NULL, NULL, NULL, 9000);
	zend_resource *res_client = zend_register_resource(ch_object, clickhouse_obj_res_num);
	zval* obj = getThis();
	zval zv_client;
	ZVAL_RES(&zv_client, res_client);
	zend_update_property(clickhouse_ce, obj, "connection", sizeof("connection") - 1, zv_client TSRMLS_CC);
}

}
