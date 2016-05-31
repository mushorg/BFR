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

#ifndef PHP_BFR_H
#define PHP_BFR_H

#include "php.h"
#include "zend.h"
#include "zend_extensions.h"
#include "ext/standard/info.h"

// ---------------------------------------------------------------------------
// Required Declarations
// ---------------------------------------------------------------------------

/* Declarations of functions to be exported. */
PHP_FUNCTION(override_function);
PHP_FUNCTION(rename_function);

PHP_MINIT_FUNCTION(bfr);
PHP_RINIT_FUNCTION(bfr);
PHP_RSHUTDOWN_FUNCTION(bfr);
PHP_MINFO_FUNCTION(bfr);

extern zend_module_entry bfr_module_entry;

#define PHP_BFR_VERSION "0.1"

ZEND_BEGIN_MODULE_GLOBALS(bfr)
ZEND_END_MODULE_GLOBALS(bfr)

PHPAPI ZEND_EXTERN_MODULE_GLOBALS(bfr)

/* Declare global structure. */
#ifdef ZTS
#define BFR_GLOBALS(v) TSRMG(bfr_globals_id, zend_bfr_globals *, v)
#else
#define BFR_GLOBALS(v) (bfr_globals.v)
#endif

#endif