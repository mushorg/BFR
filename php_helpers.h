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

#ifndef PHP_HELPERS_H
#define PHP_HELPERS_H

#include "php.h"
#include "zend.h"

zend_function *duplicate_function(zend_function *func);

#endif