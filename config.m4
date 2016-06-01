PHP_ARG_ENABLE(bfr, whether to enable BFR support, [ --enable-bfr    Enable BFR support])

if test "$PHP_BFR" = "yes"; then
  AC_DEFINE(HAVE_BFR, 1, [Whether you have BFR])
  PHP_NEW_EXTENSION(bfr, php_bfr.c php_helpers.c, $ext_shared,, -Werror -Wall)
fi