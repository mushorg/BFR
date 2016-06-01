#ifndef PHP_HELPERS_H
#define PHP_HELPERS_H

#include "php.h"
#include "zend.h"

zend_function *duplicate_function(zend_function *func);

#endif