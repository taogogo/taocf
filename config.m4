dnl $Id$
dnl config.m4 for extension taocf

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(taocf, for taocf support,
Make sure that the comment is aligned:
[  --with-taocf             Include taocf support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(taocf, whether to enable taocf support,
dnl Make sure that the comment is aligned:
dnl [  --enable-taocf           Enable taocf support])

if test "$PHP_TAOCF" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-taocf -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/taocf.h"  # you most likely want to change this
  dnl if test -r $PHP_TAOCF/$SEARCH_FOR; then # path given as parameter
  dnl   TAOCF_DIR=$PHP_TAOCF
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for taocf files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TAOCF_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TAOCF_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the taocf distribution])
  dnl fi

  dnl # --with-taocf -> add include path
  dnl PHP_ADD_INCLUDE($TAOCF_DIR/include)

  dnl # --with-taocf -> check for lib and symbol presence
  dnl LIBNAME=taocf # you may want to change this
  dnl LIBSYMBOL=taocf # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TAOCF_DIR/lib, TAOCF_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_TAOCFLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong taocf lib version or lib not found])
  dnl ],[
  dnl   -L$TAOCF_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(TAOCF_SHARED_LIBADD)

  PHP_NEW_EXTENSION(taocf, taocf.c, $ext_shared)
fi
