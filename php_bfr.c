/*
   +----------------------------------------------------------------------+
   | Better Function Replacer                                             |
   | based on APD Profiler & Debugger                                     |
   +----------------------------------------------------------------------+
   | Copyright (c) 2001-2003 Community Connect Inc.                       |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.02 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/2_02.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Lukas Rist <glaslos@gmail.com>                              |
   |          Daniel Cowgill <dcowgill@communityconnect.com>              |
   |          George Schlossnagle <george@lethargy.org>                   |
   |          Sterling Hughes <sterling@php.net>                          |
   +----------------------------------------------------------------------+
*/

#include "php_bfr.h"

ZEND_DECLARE_MODULE_GLOBALS(bfr);

/* --------------------------------------------------------------------------
   List of exported functions
   --------------------------------------------------------------------------- */
zend_function_entry bfr_functions[] = {
	PHP_FE(override_function, NULL)
	PHP_FE(rename_function, NULL)
	{NULL, NULL, NULL}
};

/* --------------------------------------------------------------------------
   Module Entry
   --------------------------------------------------------------------------- */
zend_module_entry bfr_module_entry = {
	STANDARD_MODULE_HEADER,
	"bfr",
	bfr_functions,
	PHP_MINIT(bfr),
	NULL,
	PHP_RINIT(bfr),
	PHP_RSHUTDOWN(bfr),
	PHP_MINFO(bfr),
	PHP_BFR_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#if COMPILE_DL_BFR
ZEND_GET_MODULE(bfr)
#endif

/* ---------------------------------------------------------------------------
   PHP Configuration Functions
   --------------------------------------------------------------------------- */

PHP_INI_BEGIN()
PHP_INI_END()

/* ---------------------------------------------------------------------------
   Module Startup and Shutdown Function Definitions
   --------------------------------------------------------------------------- */

static void php_bfr_init_globals(zend_bfr_globals *bfr_globals)
{
	memset(bfr_globals, 0, sizeof(zend_bfr_globals));
}

PHP_MINIT_FUNCTION(bfr)
{
	ZEND_INIT_MODULE_GLOBALS(bfr, php_bfr_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}

PHP_RINIT_FUNCTION(bfr)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(bfr)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(bfr)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Better Function Replacer (BFR)", "Enabled");
	php_info_print_table_row(2, "BFR Version", PHP_BFR_VERSION);
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

/* ---------------------------------------------------------------------------
   PHP Extension Functions
   --------------------------------------------------------------------------- */

#define TEMP_OVRD_FUNC_NAME "__overridden__"
PHP_FUNCTION(override_function)
{
	char *eval_code,*eval_name;
	int eval_code_length, retval;
	zval *z_function_name, *z_function_args, *z_function_code;

	if (ZEND_NUM_ARGS() != 3 ||
		zend_get_parameters(ht, 3, &z_function_name, &z_function_args,
							   &z_function_code) == FAILURE)
		{
			ZEND_WRONG_PARAM_COUNT();
		}

	convert_to_string_ex(&z_function_name);
	convert_to_string_ex(&z_function_args);
	convert_to_string_ex(&z_function_code);

	eval_code_length = sizeof("function " TEMP_OVRD_FUNC_NAME)
		+ Z_STRLEN_P(z_function_args)
		+ 2 /* parentheses */
		+ 2 /* curlies */
		+ Z_STRLEN_P(z_function_code);
	eval_code = (char *) emalloc(eval_code_length);
	sprintf(eval_code, "function " TEMP_OVRD_FUNC_NAME "(%s){%s}",
			Z_STRVAL_P(z_function_args), Z_STRVAL_P(z_function_code));
	eval_name = zend_make_compiled_string_description("runtime-created override function" TSRMLS_CC);
	retval = zend_eval_string(eval_code, NULL, eval_name TSRMLS_CC);
	efree(eval_code);
	efree(eval_name);

	if (retval == SUCCESS) {
		zend_function *func;

		if (zend_hash_find(EG(function_table), TEMP_OVRD_FUNC_NAME,
						   sizeof(TEMP_OVRD_FUNC_NAME), (void **) &func) == FAILURE)
			{
				zend_error(E_ERROR, "%s() temporary function name not present in global function_table", get_active_function_name(TSRMLS_C));
				RETURN_FALSE;
			}
		function_add_ref(func);
		zend_hash_del(EG(function_table), Z_STRVAL_P(z_function_name),
					  Z_STRLEN_P(z_function_name) + 1);
		if(zend_hash_add(EG(function_table), Z_STRVAL_P(z_function_name),
						 Z_STRLEN_P(z_function_name) + 1, func, sizeof(zend_function),
						 NULL) == FAILURE)
			{
				RETURN_FALSE;
			}
		RETURN_TRUE;
	}
	else {
		RETURN_FALSE;
	}
}

PHP_FUNCTION(rename_function)
{
	zval *z_orig_fname, *z_new_fname;
	zend_function *func, *dummy_func;

	if( ZEND_NUM_ARGS() != 2 ||
		zend_get_parameters(ht, 2, &z_orig_fname, &z_new_fname) == FAILURE )
		{
			ZEND_WRONG_PARAM_COUNT();
		}

	convert_to_string_ex(&z_orig_fname);
	convert_to_string_ex(&z_new_fname);

	if(zend_hash_find(EG(function_table), Z_STRVAL_P(z_orig_fname),
					  Z_STRLEN_P(z_orig_fname) + 1, (void **) &func) == FAILURE)
		{
			zend_error(E_WARNING, "%s(%s, %s) failed: %s does not exist!",
					   get_active_function_name(TSRMLS_C),
					   Z_STRVAL_P(z_orig_fname),  Z_STRVAL_P(z_new_fname),
					   Z_STRVAL_P(z_orig_fname));
			RETURN_FALSE;
		}
	if(zend_hash_find(EG(function_table), Z_STRVAL_P(z_new_fname),
					  Z_STRLEN_P(z_new_fname) + 1, (void **) &dummy_func) == SUCCESS)
		{
			zend_error(E_WARNING, "%s(%s, %s) failed: %s already exists!",
					   get_active_function_name(TSRMLS_C),
					   Z_STRVAL_P(z_orig_fname),  Z_STRVAL_P(z_new_fname),
					   Z_STRVAL_P(z_new_fname));
			RETURN_FALSE;
		}
	if(zend_hash_add(EG(function_table), Z_STRVAL_P(z_new_fname),
					 Z_STRLEN_P(z_new_fname) + 1, func, sizeof(zend_function),
					 NULL) == FAILURE)
		{
			zend_error(E_WARNING, "%s() failed to insert %s into EG(function_table)",
					   get_active_function_name(TSRMLS_C),
					   Z_STRVAL_P(z_new_fname));
			RETURN_FALSE;
		}
	if(zend_hash_del(EG(function_table), Z_STRVAL_P(z_orig_fname),
					 Z_STRLEN_P(z_orig_fname) + 1) == FAILURE)
		{
			zend_error(E_WARNING, "%s() failed to remove %s from function table",
					   get_active_function_name(TSRMLS_C),
					   Z_STRVAL_P(z_orig_fname));
			zend_hash_del(EG(function_table), Z_STRVAL_P(z_new_fname),
						  Z_STRLEN_P(z_new_fname) + 1);
			RETURN_FALSE;
		}
	RETURN_TRUE;
}

// ---------------------------------------------------------------------------
// Zend Extension Functions
// ---------------------------------------------------------------------------

int bfr_zend_startup(zend_extension *extension)
{
	TSRMLS_FETCH();
	return zend_startup_module(&bfr_module_entry);
}

ZEND_DLEXPORT void bfr_zend_shutdown(zend_extension *extension)
{
	/* Do nothing. */
}

#ifndef ZEND_EXT_API
#define ZEND_EXT_API	ZEND_DLEXPORT
#endif
ZEND_EXTENSION();

ZEND_DLEXPORT zend_extension zend_extension_entry = {
	"Better Function Replacer (BFR)",
	PHP_BFR_VERSION,
	"Lukas Rist",
	"http://mushmush.org/",
	"Copyright (C) 2015",
	bfr_zend_startup,
	bfr_zend_shutdown,
	NULL,      // activate_func_t
	NULL,      // deactivate_func_t
	NULL,      // message_handler_func_t
	NULL,      // op_array_handler_func_t
	NULL,      // statement_handler_func_t
	NULL,      // fcall_begin_handler_func_t
	NULL,      // fcall_end_handler_func_t
	NULL,      // op_array_ctor_func_t
	NULL,      // op_array_dtor_func_t
	NULL,      // api_no_check
	COMPAT_ZEND_EXTENSION_PROPERTIES
};

/**
 * Local Variables:
 * indent-tabs-mode: t
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600:fdm=marker
 * vim:noet:sw=4:ts=4
 */
