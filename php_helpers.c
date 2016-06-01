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