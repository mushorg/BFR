/* 
   +----------------------------------------------------------------------+
   | Better Function Replacer											  |
   | based on APD Profiler & Debugger     								  |
   +----------------------------------------------------------------------+
   | Copyright (c) 2001-2003 Community Connect Inc.  					  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 2.02 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available at through the world-wide-web at                           |
   | http://www.php.net/license/2_02.txt.                                 |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Lukas Rist <glaslos@gmail.com>							  |
   |          Daniel Cowgill <dcowgill@communityconnect.com>              |
   |          George Schlossnagle <george@lethargy.org>                   |
   |          Sterling Hughes <sterling@php.net>                          |
   +----------------------------------------------------------------------+
*/

#ifndef PHP_BFR_H
#define PHP_BFR_H

#include "php.h"
#include "php_config.h"

#include "php_ini.h"
#include "php_globals.h"
#include "ext/standard/php_string.h"

#include "zend.h"
#include "zend_API.h"
#include "zend_execute.h"
#include "zend_compile.h"
#include "zend_extensions.h"
 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <unistd.h>


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
#define bfr_module_ptr &bfr_module_entry

#define PHP_BFR_VERSION "0.1"

#define FUNCTION_TRACE 1
#define ARGS_TRACE 2
#define ASSIGNMENT_TRACE 4
#define STATEMENT_TRACE 8
#define MEMORY_TRACE 16
#define TIMING_TRACE 32
#define SUMMARY_TRACE 64
#define ERROR_TRACE 128
#define PROF_TRACE 256


ZEND_BEGIN_MODULE_GLOBALS(bfr)
	int counter;
	void* stack;
	HashTable function_summary;
	HashTable file_summary;
	char* dumpdir; /* directory for dumping seesion traces to */
	FILE* dump_file; /* FILE for dumping session traces to */
	FILE* pprof_file; /* File for profiling output */
	int dump_sock_id; /* Socket for dumping data to */
	struct timeval first_clock;
	struct timeval last_clock;
	struct rusage first_ru;
	struct rusage last_ru;
	long entry_memory_usage;
	int function_index;                /* current index of functions for pprof tracing */
	int file_index;                /* current index of functions for pprof tracing */
	int current_file_index;
	long pproftrace;           /* Flag for whether we are doing profiling */
	void* last_mem_header;		/* tail of persistent zend_mem_header list */
	void* last_pmem_header;		/* tail of persistent zend_mem_header list */
	int interactive_mode;     /* is interactive mode on */
	int ignore_interactive;   /* ignore interactive mode flag for executing php from the debugger*/
	int allocated_memory;
	int statement_tracing;
ZEND_END_MODULE_GLOBALS(bfr)

PHPAPI ZEND_EXTERN_MODULE_GLOBALS(bfr)

/* Declare global structure. */

#ifdef ZTS
#define BFR_GLOBALS(v) TSRMG(bfr_globals_id, zend_bfr_globals *, v)
#else
#define BFR_GLOBALS(v) (bfr_globals.v)
#endif

#define phpext_bfr_ptr bfr_module_ptr

#endif