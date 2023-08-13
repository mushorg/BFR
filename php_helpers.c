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

#include "php_helpers.h"

zend_function *duplicate_function(zend_function *func)
{
  zend_function *new_function;

  if (func->type != ZEND_INTERNAL_FUNCTION)
  {
    new_function = zend_arena_alloc(&CG(arena), sizeof(zend_op_array));
    memcpy(new_function, func, sizeof(zend_op_array));

    function_add_ref(new_function);

    return new_function;
  }

  new_function = zend_arena_alloc(&CG(arena), sizeof(zend_internal_function));
  memcpy(new_function, func, sizeof(zend_internal_function));
  new_function->common.fn_flags |= ZEND_ACC_ARENA_ALLOCATED;

  function_add_ref(new_function);

  return new_function;
}
