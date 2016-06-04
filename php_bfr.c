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
   |          Nikita Tokarchuk <nikita@tokarch.uk>                        |
   +----------------------------------------------------------------------+
 */

#include "php_bfr.h"
#include "php_helpers.h"

ZEND_DECLARE_MODULE_GLOBALS(bfr);

/* --------------------------------------------------------------------------
   List of exported functions
   --------------------------------------------------------------------------- */
zend_function_entry bfr_functions[] = {
	PHP_FE(override_function, NULL)
	PHP_FE(rename_function, NULL)
	{
		NULL, NULL, NULL
	}
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
#define TEMP_OVRD_FUNC_HEADER "function " TEMP_OVRD_FUNC_NAME
#define TEMP_OVRD_FUNC_PATTERN TEMP_OVRD_FUNC_HEADER "(%s){%s}"
#define TEMP_OVRD_FUNC_DESC "runtime-created override function"

PHP_FUNCTION(override_function)
{
	char *eval_code, *eval_name;
	int eval_code_length, retval;
	char *z_function_name, *z_function_args, *z_function_code;
	size_t function_name_len, function_args_len, function_code_len;
	zend_function *func, *func_dup;

	if (ZEND_NUM_ARGS() != 3 ||
		zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sss",
							&z_function_name, &function_name_len,
							&z_function_args, &function_args_len,
							&z_function_code, &function_code_len) == FAILURE)
	{
		ZEND_WRONG_PARAM_COUNT();
	}

	eval_code_length = sizeof(TEMP_OVRD_FUNC_HEADER)
		+ function_args_len
		+ 2 /* parentheses */
		+ 2 /* curlies */
		+ function_code_len;

	eval_code = (char *) emalloc(eval_code_length);
	sprintf(eval_code, TEMP_OVRD_FUNC_PATTERN, z_function_args, z_function_code);
	eval_name = zend_make_compiled_string_description(TEMP_OVRD_FUNC_DESC TSRMLS_CC);
	retval = zend_eval_string(eval_code, NULL, eval_name TSRMLS_CC);
	efree(eval_code);
	efree(eval_name);

	if (retval != SUCCESS)
	{
		zend_error(E_ERROR, "%s() failed to eval temporary function",
				get_active_function_name(TSRMLS_C));

		RETURN_FALSE;
	}

	if ((func = zend_hash_str_find_ptr(EG(function_table),
									TEMP_OVRD_FUNC_NAME, sizeof(TEMP_OVRD_FUNC_NAME) - 1)) == NULL)
	{
		zend_error(E_ERROR, "%s() temporary function name not present in global function_table",
				get_active_function_name(TSRMLS_C));

		RETURN_FALSE;
	}

	func_dup = duplicate_function(func);

	if (zend_hash_str_exists(EG(function_table),
							z_function_name, function_name_len))
	{
		zend_hash_str_del(EG(function_table),
						z_function_name, function_name_len);
	}

	if (zend_hash_str_add_new_ptr(EG(function_table),
								z_function_name, function_name_len,
								func_dup) == NULL)
	{
		zend_error(E_ERROR, "%s() failed to add function",
				get_active_function_name(TSRMLS_C));

		RETURN_FALSE;
	}

	if (zend_hash_str_del(EG(function_table), TEMP_OVRD_FUNC_NAME,
						sizeof(TEMP_OVRD_FUNC_NAME) - 1) == FAILURE)
	{
		zend_error(E_ERROR, "%s() failed to delete temporary function",
				get_active_function_name(TSRMLS_C));

		zend_hash_str_del(EG(function_table),
						z_function_name, function_name_len);

		RETURN_FALSE;
	}
}

PHP_FUNCTION(rename_function)
{
	char *z_orig_fname, *z_new_fname;
	size_t orig_fname_len, new_fname_len;
	zend_function *func, *func_dup;

	if (ZEND_NUM_ARGS() != 2 ||
		zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
							&z_orig_fname, &orig_fname_len,
							&z_new_fname, &new_fname_len) == FAILURE)
	{
		ZEND_WRONG_PARAM_COUNT();
	}

	if ((func = zend_hash_str_find_ptr(EG(function_table),
									z_orig_fname, orig_fname_len)) == NULL)
	{
		zend_error(E_WARNING, "%s(%s, %s) failed: %s does not exist!",
				get_active_function_name(TSRMLS_C),
				z_orig_fname, z_new_fname, z_orig_fname);

		RETURN_FALSE;
	}
	if (zend_hash_str_exists(EG(function_table), z_new_fname, new_fname_len))
	{
		zend_error(E_WARNING, "%s(%s, %s) failed: %s already exists!",
				get_active_function_name(TSRMLS_C),
				z_orig_fname, z_new_fname, z_new_fname);

		RETURN_FALSE;
	}

	func_dup = duplicate_function(func);

	if (zend_hash_str_add_ptr(EG(function_table), z_new_fname, new_fname_len, func_dup) == NULL)
	{
		zend_error(E_WARNING, "%s() failed to insert %s into EG(function_table)",
				get_active_function_name(TSRMLS_C), z_new_fname);

		RETURN_FALSE;
	}
	if (zend_hash_str_del(EG(function_table), z_orig_fname, orig_fname_len) == FAILURE)
	{
		zend_error(E_WARNING, "%s() failed to remove %s from function table",
				get_active_function_name(TSRMLS_C), z_orig_fname);

		zend_hash_str_del(EG(function_table), z_new_fname, new_fname_len);

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
#define ZEND_EXT_API ZEND_DLEXPORT
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
	NULL, // activate_func_t
	NULL, // deactivate_func_t
	NULL, // message_handler_func_t
	NULL, // op_array_handler_func_t
	NULL, // statement_handler_func_t
	NULL, // fcall_begin_handler_func_t
	NULL, // fcall_end_handler_func_t
	NULL, // op_array_ctor_func_t
	NULL, // op_array_dtor_func_t
	NULL, // api_no_check
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
