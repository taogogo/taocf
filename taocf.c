/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_taocf.h"
//php_var_dump等函数的定义文件
#include "ext/standard/php_var.h"

/* If you declare any globals in php_taocf.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(taocf)
*/

/* True global resources - no need for thread safety here */
static int le_taocf;

/* {{{ taocf_methods[]
 * 每个类都会有这样一个entry
 * Every user visible function must have an entry in taocf_methods[].
 */
const zend_function_entry taocf_methods[] = {
	//声明Common::getConfig()方法为公开、静态方法
	PHP_ME(Common, getConfig, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Common, debugDump, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Common, autoload, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	//PHP_ME(Common, autoload, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END	/* Must be the last line in taocf_methods[] */
};
/* }}} */

/* {{{ taocf_module_entry
 */
zend_module_entry taocf_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"taocf",
	taocf_methods,
	PHP_MINIT(taocf),
	PHP_MSHUTDOWN(taocf),
	PHP_RINIT(taocf),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(taocf),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(taocf),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TAOCF
ZEND_GET_MODULE(taocf)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("taocf.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_taocf_globals, taocf_globals)
    STD_PHP_INI_ENTRY("taocf.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_taocf_globals, taocf_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_taocf_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_taocf_init_globals(zend_taocf_globals *taocf_globals)
{
	taocf_globals->global_value = 0;
	taocf_globals->global_string = NULL;
}
*/
/* }}} */

//类结构变量，类实例化后存在这个结构体内
zend_class_entry *Common_ce;

/* {{{ PHP_MINIT_FUNCTION
 * 扩展加载时执行的方法
 */
PHP_MINIT_FUNCTION(taocf)
{
	//定义版本号常量
	REGISTER_STRING_CONSTANT("TCF_VERSION", "0.1", CONST_CS | CONST_PERSISTENT);
    //注册ini的配置(这样phpinfo中才可以看到)
    //REGISTER_INI_ENTRIES();
    zend_class_entry Common_entry;
    //第二个参数为类名，第三个参数为函数表
    INIT_CLASS_ENTRY(Common_entry, "Common", taocf_methods);
    Common_ce = zend_register_internal_class_ex(&Common_entry, NULL, NULL TSRMLS_CC);//注册类
   
   

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(taocf)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(taocf)
{
	zval app_root_path; 
	if(zend_get_constant("ROOT_PATH", sizeof("ROOT_PATH")-1, &app_root_path TSRMLS_DC)){
    	//声明autoload函数
	    zend_eval_string( "Common::autoload();",NULL, "class autoload" TSRMLS_CC); 
    }
	
	return SUCCESS;
}

/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(taocf)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(taocf)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "taocf(php framework extension) support", "enabled");
	//读取常量
	zval tcf_version; 
    if(zend_get_constant("TCF_VERSION", sizeof("TCF_VERSION")-1, &tcf_version TSRMLS_DC)){
    	php_info_print_table_row(2, "version", Z_STRVAL(tcf_version));
    }
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* Common::autoload类定义
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_METHOD(Common,autoload)
{
    	//声明autoload函数
	    zend_eval_string( "function __autoload($className){include_once(ROOT_PATH.'/'.implode('/',explode('_',$className)).'.class.php');};",NULL, "class autoload" TSRMLS_CC); 
	return SUCCESS;
}

/* }}} */

/* {{{ Common::getConfig类定义
 */
PHP_METHOD(Common, getConfig) {

	//从文件中读取配置，参考：http://lxr.php.net/xref/PHP_5_3/ext/standard/file.c#529

    char *arg = NULL;
	int arg_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
	//zend_printf("a arg:%s",(arg));
	
	zval *args[1];
	//第三个 值为 1 将先申请一块新内存并赋值该字符串，
	//然后把新内存的地址复制给pzv， 为 0 时则是直接把str的地址赋值给zval。
    MAKE_STD_ZVAL(args[0]);
    ZVAL_STRING(args[0], arg, 1);

	//存储读取的常量
	zval app_root_path; 
	//如果常量不存在，则报错，并返回NULL
	if(!zend_get_constant("ROOT_PATH", sizeof("ROOT_PATH")-1, &app_root_path TSRMLS_DC)){
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "ROOT_PATH SHOULD BE DEFINED");	
    	RETURN_NULL();
    }

    //结果存放变量
    zval *result; 
	MAKE_STD_ZVAL(result);
    //存放要执行的代码
	char *code;
    //拼接eval代码
	spprintf(&code, 0, "require('%s/config/%s');",Z_STRVAL(app_root_path), Z_STRVAL_P(args[0]));
	zend_eval_string(code, result, "Require Config File" TSRMLS_CC); 
	RETURN_ZVAL(result, 1, 0); 

	/*
	//@deprecated 原来流程是：调用php函数，执行字符串连接，然后在eval执行
	//现在使用上面的代码就可以了
	zval fname,evalHead;
	ZVAL_STRING(&fname, "file_get_contents", 0);
    ZVAL_STRING(&evalHead, "?>", 0);
    if (call_user_function(EG(function_table), NULL, &fname, return_value, 1, args TSRMLS_CC) == SUCCESS) {
	    
	    //传统操作内存连接字符串方法
	    
		uint res_len = Z_STRLEN_P(&evalHead) + Z_STRLEN_P(return_value);
	 
		Z_STRVAL_P(return_value) = erealloc(Z_STRVAL_P(return_value), res_len+1);
	 	char *tmp=NULL;
	 	tmp=Z_STRVAL_P(return_value);

	 	memcpy(Z_STRVAL_P(return_value), Z_STRVAL_P(&evalHead), Z_STRLEN_P(&evalHead));
	 	memcpy(Z_STRVAL_P(return_value)+Z_STRLEN_P(&evalHead), tmp, strlen(tmp));
	 	tmp = NULL;
		//memcpy(Z_STRVAL_P(return_value)+Z_STRLEN_P(return_value), Z_STRVAL_P(&evalHead), Z_STRLEN_P(&evalHead));
		Z_STRVAL_P(return_value)[res_len]=0;
		Z_STRLEN_P(return_value) = res_len;
		
		//使用函数
		
		char *code;
		int len;
		len = spprintf(&code, 0, "?>%s", Z_STRVAL_P(return_value));
		//ZVAL_STRING(return_value, code, 0);
		

  //       if (zend_eval_string(code, result, "Cookbook eval'd code") == FAILURE) {
  //   		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Error executing provided code");
		// 	RETVAL_FALSE; 
		// }
		
        //zend_printf("call a function%s",Z_STRVAL_P(return_value));

       //zval_ptr_dtor(&args[0]);
    }
    */
}

/* }}} */

/* {{{ Common::debugDump类定义,用于调试打印
 */
PHP_METHOD(Common, debugDump) {
 	//获取可变参数
	int i, argc = ZEND_NUM_ARGS();
    zval ***args;
    args = (zval ***)safe_emalloc(argc, sizeof(zval **), 0);
    if (ZEND_NUM_ARGS() == 0 || zend_get_parameters_array_ex(argc, args) == FAILURE) {
        efree(args);
        WRONG_PARAM_COUNT;
    }
   
    //如果全局变量表中没有_SERVER，则初始化这个全局变量*?*
	if (!zend_hash_exists(&EG(symbol_table), "_SERVER", 8)) {
	    zend_auto_global* auto_global;
	    if (zend_hash_find(CG(auto_globals), "_SERVER", 8, (void **)&auto_global) != FAILURE) {
	        auto_global->armed = auto_global->auto_global_callback(auto_global->name, auto_global->name_len TSRMLS_CC);
	    }
	}

	//从$_SERVER获取argv下标内容，存入arr
	zval** arr;
	char *code="";
	int cliArgc = 0;
	if (zend_hash_find(&EG(symbol_table), "_SERVER", 8, (void **)&arr) != FAILURE) {
	    HashTable* ht = Z_ARRVAL_P(*arr);
	    zval **argvVal,**argcVal;
	    //将$_SERVER的argv下标放入argvVal
	    if (zend_hash_find(ht, "argv", 5, (void**)&argvVal) != FAILURE) {
	    	//转换为数组格式
	    	convert_to_array_ex(argvVal);
	    	//获取argv的有几个下标
	    	long count;
	    	count = zend_hash_num_elements(Z_ARRVAL_PP(argvVal));
	        //zend_printf("type is %d，total is %d",Z_TYPE_PP(argvVal),count);
	        //重置argv下标指针到数组开头
		    zend_hash_internal_pointer_reset(Z_ARRVAL_PP(argvVal));
		    zval **z_item,**tmpZitem;
		    long i;
		    //遍历获取下标内容，存入code变量内，并将指针后移一位
		    for (i = 0; i < count; i ++)
		    {
		        /*
		        char* key;
		        unsigned long idx;
		        zend_printf("get it:%d",zend_hash_get_current_key(Z_ARRVAL_PP(argvVal), &key, &idx, 0));
		        if (zend_hash_get_current_key(Z_ARRVAL_PP(argvVal), &key, &idx, 0) == HASH_KEY_IS_STRING)
		        {
		            zend_hash_get_current_data(Z_ARRVAL_PP(argvVal), (void**) &z_item);
		            convert_to_string_ex(z_item);
		            zend_printf("item is %s",Z_STRVAL_PP(z_item));
		        } else {
		            // KEY为数字
		            zend_hash_get_current_data(Z_ARRVAL_PP(argvVal), (void**) &z_item);
		            convert_to_string_ex(z_item);
		            zend_printf("item is not string%s",Z_STRVAL_PP(z_item));
		        }
		        */
		        //将当前下标的数据赋给z_item
	            zend_hash_get_current_data(Z_ARRVAL_PP(argvVal), (void**) &z_item);
	            //复制一份item，然后进行操作
	            tmpZitem = z_item;
	            zval_copy_ctor(*tmpZitem);
		        //将z_item的内容转换为字符串
	            convert_to_string_ex(tmpZitem);
	            //zend_printf("%s",Z_STRVAL_PP(z_item));
	            //拼接code字符串，用于后面展示
	            spprintf(&code, 0, "%s%s|",code,Z_STRVAL_PP(tmpZitem));
	            //将指针后移一位
		        zend_hash_move_forward(Z_ARRVAL_PP(argvVal));
		    }
	    }
	    //将argc下标数据取出
	    if (zend_hash_find(ht, "argc", 5, (void**)&argcVal) != FAILURE) {
	        cliArgc = Z_LVAL_PP(argcVal);
	    }
	}

	//放在这里，和代码无关：清理分配的内存zval_ptr_dtor
	//如果命令行参数数量不为0，则是cli环境，打印出cli参数
	//否则，是web环境，添加<pre>标签
	if(cliArgc != 0) {
		zend_printf("\n################\n");
        zend_printf("%s",code);
		efree(code);
        zend_printf("\n################\n");
	}else{
		zend_printf("<pre>");
	}
	//遍历方法的参数，并打印,在参数前添加#d，方便查看是哪个参数。
	for (i=0; i<argc; i++) {
	 	zend_printf("#%d	",i+1);
        php_var_dump(args[i], 1 TSRMLS_CC);
    }
    efree(args);
}

/* }}} */

/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
