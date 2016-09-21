AC_PREREQ([2.69])
AC_INIT([cpareia], [1.1.0], [michel.boaventura@gmail.com])
AC_CONFIG_SRCDIR([src/cpareia.c])
AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])

AM_SILENT_RULES([yes])

AC_CONFIG_FILES([Makefile src/Makefile])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h stddef.h stdint.h stdlib.h string.h strings.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([bzero memset munmap strdup strndup strtoull])

AC_SEARCH_LIBS(
  [log2],
  [m],
  [],
  [AC_MSG_ERROR([unable to find the log2() function])])

AC_SEARCH_LIBS(
  [pthread_create],
  [pthread],
  [],
  [AC_MSG_ERROR([unable to find the pthread_create() function])])

PKG_CHECK_MODULES([LIBXML], [libxml-2.0])
PKG_CHECK_MODULES([ZLIB], [zlib])

#Checks for cmocka, used in tests
AC_ARG_WITH([cmocka], AS_HELP_STRING([--without-cmocka], [Disable tests]))

AS_IF(
  [test "x$with_cmocka" != "xno"],
  [PKG_CHECK_MODULES(
    [CMOCKA], [cmocka >= 1.0.0], [have_cmocka=yes], [have_cmocka=no])],
  [have_cmocka=no])

AS_IF(
  [test "x$have_cmocka" = "xyes"],
  [AC_CONFIG_FILES([tests/Makefile])],
  [AS_IF(
    [test "x$with_cmocka" = "xyes"],
    [AC_MSG_ERROR([cmocka requested but not found])])])

AM_CONDITIONAL([CMOCKA], [test "x$have_cmocka" = "xyes"])

#Checks for tcmalloc, which improves performance
AC_ARG_WITH(
  [tcmalloc],
  AS_HELP_STRING([--without-tcmalloc], [Disable tcmalloc usage]))

AS_IF(
  [test "x$with_tcmalloc" != "xno"],
  [PKG_CHECK_MODULES(
    [TCMALLOC],
    [libtcmalloc_minimal], [have_tcmalloc=yes], [have_tcmalloc=no])],
  [have_tcmalloc=no])

AS_IF(
  [test "x$have_tcmalloc" = "xyes"],
  [],
  [AS_IF(
    [test "x$with_tcmalloc" = "xyes"],
    [AC_MSG_ERROR([tcmalloc requested but not found])])])

AM_CONDITIONAL([TCMALLOC], [test "x$have_tcmalloc" = "xyes"])

#Checks for sse2, which improves performance
AC_ARG_WITH([sse2], AS_HELP_STRING([--without-sse2], [Disable sse2 usage]))

AS_IF(
  [test "x$with_sse2" != "xno"],
  [
    AC_LINK_IFELSE(
      [
        AC_LANG_PROGRAM(
          [
            #include <emmintrin.h>
          ],
          [__m128i input1;
            unsigned char buf1[[128]];
            input1 = _mm_loadu_si128((const __m128i *)buf1);
            return(0);]
        )
      ],
      [have_sse2=yes],
      [have_sse2=no]
    )
  ],
  [have_sse2=no])

AS_IF(
  [test "x$have_sse2" = "xyes"],
  [],
  [AS_IF(
    [test "x$with_sse2" = "xyes"],
    [AC_MSG_ERROR([sse2 requested but not supported])])])

AM_CONDITIONAL([SSE2], [test "x$have_sse2" = "xyes"])

AC_OUTPUT
