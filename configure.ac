AC_INIT([sbrl], 1.0)
 
## Use gsl-config to find arguments for compiler and linker flags
##
## Check for non-standard programs: gsl-config(1)
AC_PATH_PROG([GSL_CONFIG], [gsl-config])
## If gsl-config was found, let's use it
if test "${GSL_CONFIG}" != ""; then
    # Use gsl-config for header and linker arguments
    GSL_CFLAGS=`${GSL_CONFIG} --cflags`
    GSL_LIBS=`${GSL_CONFIG} --libs`
else
    AC_MSG_ERROR([gsl-config not found, is GSL1 installed?])
fi

## Now for GMP
AC_C_INLINE

#
# Check to see if GMP exists.
#

GMP_VER=0
GMP_INC=""
GMP_LIBS=""
GMP_DIR=""

# First, parse any --with-gmp args.

AC_ARG_WITH(gmp,
  [  --with-gmp[=DIR]        enable gmp support [DIR=yes, no, or gmp dir]]
  [                          (e.g. ../gmp-1.3.2, /usr/gnu /usr/local/gnu)],
  [
    case "$withval" in
      no)
        GMP_VER=-1
        ;;
      yes)
        ;;
      *)
        GMP_DIR="$withval"
        ;;
    esac
  ]
)

# Paths to search for GMP header and library files.  List only the parent
# directory of the include/ and lib/ dirs, e.g. "/usr/gnu" instead of
# both "/usr/gnu/include" and "/usr/gnu/lib"

GMP_PATH="/usr/local/Cellar/gmp/6.1.1 \
          /usr/local/Cellar/gmp/6.1.0 \
          /usr/local/Cellar/gmp/6.0.0 \
          /usr/local/Cellar/gmp/5.1.3 \
          /usr/local/Cellar/gmp/5.1.2 \
          /usr/local/Cellar/gmp/5.1.1 \
          /usr/local/Cellar/gmp/5.1.0 \
          /usr/local/Cellar/gmp/5.0.5 \
          /usr/local/Cellar/gmp/5.0.4 \
          /usr/local/Cellar/gmp/5.0.3 \
          /usr/local/Cellar/gmp/5.0.2 \
          /usr/local/Cellar/gmp/5.0.1 \
          /usr/local/Cellar/gmp/5.0.0 \
          /usr/local/Cellar/gmp/4.3.2 \
          /usr/local/Cellar/gmp/4.3.1 \
          /usr/local/Cellar/gmp/4.3.0 \
          /usr/local/Cellar/gmp/4.2.4 \
          /usr/local/Cellar/gmp/4.2.3 \
          /usr/local/Cellar/gmp/4.2.2 \
          /usr/local/Cellar/gmp/4.2.1 \
          /usr/local/Cellar/gmp/4.2.0 \
          /usr/gnu /usr/unsupported /usr/local/gnu \
          /usr/local /opt/local \
          /usr/lib64 \
          $srcdir/gmp"

# If GMP is not disabled...

if test $GMP_VER != -1; then

  # If GMP directory was specified, do some smart checking
  if test "$GMP_DIR" != ""; then
    # prefix the search path with the provided path
    GMP_PATH="$GMP_DIR $GMP_PATH"
  fi

  # search for the header file
  AC_MSG_CHECKING(for local gmp.h)

  for path in $GMP_PATH; do
    AC_MSG_NOTICE(searching $path)
    if test -r $path/gmp.h; then
      GMP_VER=1
      GMP_INC="-I$path"
      break
    fi
    if test -r $path/include/gmp.h; then
      GMP_VER=1
      GMP_INC="-I$path/include"
      break
    fi
  done

  if test $GMP_VER = 1; then
    AC_MSG_RESULT(found in $GMP_INC)
  else
    AC_MSG_RESULT(not found)
  fi

  # search for the library file only if header was found
  if test $GMP_VER = 1; then

    GMP_VER=0

    AC_MSG_CHECKING(for local libgmp.a)

    for path in $GMP_PATH; do
      if test -r $path/libgmp.a; then
        GMP_VER=1
        GMP_LIBS="-L$path -lgmp"
        break
      fi
      if test -r $path/lib/libgmp.a; then
        GMP_VER=1
        GMP_LIBS="-L$path/lib -lgmp"
        break
      fi
    done
    if test $GMP_VER = 1; then
      AC_MSG_RESULT(found in $GMP_LIBS)
    else
      AC_MSG_RESULT(not found)
    fi

#    # Check for GMP2
#    if test $GMP_VER = 1; then
#      LDOLD="$LDFLAGS"
#      LDFLAGS="$GMP_LIBS -lgmp"
#      AC_CHECK_LIB(gmp, mpz_fdiv_q_ui, AC_DEFINE(HAVE_LIB_GMP2) GMP_VER=2)
#      if test $ac_cv_lib_gmp_mpz_fdiv_q_ui = no; then
#        dnl with gmp3 it's a #define, use real function name
#        AC_CHECK_LIB(gmp, __gmpz_fdiv_q_ui, AC_DEFINE(HAVE_LIB_GMP2) GMP_VER=2)
#      fi
#      LDFLAGS="$LDOLD"
#    fi

  fi

  # If no GMP found yet, search the default system locations for it
  # We want any local copy of GMP to override the system GMP

  if test $GMP_VER = 0; then

    AC_CHECK_HEADER(gmp.h, AC_DEFINE(HAVE_GMP_H) GMP_VER=1)

    if test $GMP_VER = 1; then
      AC_CHECK_LIB(gmp, mpz_init, AC_DEFINE(HAVE_LIB_GMP), GMP_VER=0)
      if test $ac_cv_lib_gmp_mpz_init = no; then
        dnl with gmp3 it's a #define, use real function name
        AC_CHECK_LIB(gmp, __gmpz_init, AC_DEFINE(HAVE_LIB_GMP2) GMP_VER=2)
      fi
    fi

#    # Check for GMP2
#    if test $GMP_VER = 1; then
#      AC_CHECK_LIB(gmp, mpz_fdiv_q_ui, AC_DEFINE(HAVE_LIB_GMP2) GMP_VER=2)
#      if test $ac_cv_lib_gmp_mpz_fdiv_q_ui = no; then
#        dnl with gmp3 it's a #define, use real function name
#        AC_CHECK_LIB(gmp, __gmpz_fdiv_q_ui, AC_DEFINE(HAVE_LIB_GMP2) GMP_VER=2)
#      fi
#    fi

  fi

fi


case $GMP_VER in
  -1)
    echo GMP support is disabled, non-GMP version of librsa will be built.
    ;;
   0)
    echo GMP not found, non-GMP version of librsa will be built.
    GMP_TARGETS="nogmp"
    ;;
   1)
    echo GMP 1.x found, building librsa.a, librsa-gmp.a, mkkey.
    GMP_TARGETS="librsa-gmp.a mkkey"
    ;;
   2)
    echo GMP 2.x found, building librsa.a, librsa-gmp.a, mkkey.
    GMP_TARGETS="librsa-gmp.a mkkey"
    ;;
   *)
    echo Unknown GMP configuration.
    ;;
esac

AC_SUBST(GSL_CFLAGS)
AC_SUBST(GSL_LIBS)
AC_SUBST(GMP_INC)
AC_SUBST(GMP_LIBS)
AC_SUBST(GMP_TARGETS)

AC_OUTPUT(src/Makevars)