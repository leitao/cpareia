AC_PREREQ([2.69])
AC_INIT([cpareia], [1.0.0], [michel.boaventura@gmail.com])
AC_CONFIG_SRCDIR([src/cpareia.c])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])

AM_SILENT_RULES([yes])

AC_CONFIG_FILES([Makefile src/Makefile])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O
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

PKG_CHECK_MODULES([LIBXML], [libxml-2.0])

AC_ARG_WITH([cmocka], AS_HELP_STRING([--without-cmocka], [Disable tests]))

AS_IF([test "x$with_cmocka" != "xno"],
      [PKG_CHECK_MODULES([CMOCKA], [cmocka >= 1.0.0], [have_cmocka=yes], [have_cmocka=no])],
      [have_cmocka=no]
)

AS_IF([test "x$have_cmocka" = "xyes"],
      [AC_CONFIG_FILES([tests/Makefile])],
      [AS_IF([test "x$with_cmocka" = "xyes"],
             [AC_MSG_ERROR([cmocka requested but not found])
      ])
])

AM_CONDITIONAL([CMOCKA], [test "x$have_cmocka" = "xyes"])

AC_OUTPUT
