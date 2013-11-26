dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(develamp, 1.0)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([1.11 silent-rules foreign subdir-objects -Wall])
AM_SILENT_RULES([yes])

AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], [AC_MSG_ERROR([compiler does not support C99])])
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_CXX_C_O
AX_CXX_COMPILE_STDCXX_11(, [mandatory])
AM_PROG_AR
AC_PROG_RANLIB

dnl Check the faust compiler version. The checks for sed and grep are optional
dnl (they are included automatically in AX_PROG_FAUST_VERSION) but the output
dnl of the configure command is more readable (the two lines of output related
dnl to faust are not split by the sed/grep checks).
AC_PROG_SED
AC_PROG_GREP
AC_PATH_PROG([FAUST],[faust])
AX_PROG_FAUST_VERSION([0.9.46],,[AC_MSG_ERROR([faust version is insufficient])])

AC_LANG([C++])

dnl gl_VALGRIND_TESTS
AC_SEARCH_LIBS([cos], [m])
AC_SEARCH_LIBS([clock_gettime], [rt],
	AM_CONDITIONAL(HAVE_CLOCK_GETTIME, true)
          AC_DEFINE(HAVE_CLOCK_GETTIME,1,[Have clock_gettime]),
	AM_CONDITIONAL(HAVE_CLOCK_GETTIME, false))

PKG_CHECK_MODULES([GTK], [gtk+-3.0 >= 3.10])
PKG_CHECK_MODULES([JACK], [jack >= 0.9])

dnl This flag is required for the GTK+ signal autoconnection to work
dnl AX_APPEND_LINK_FLAGS([-export-dynamic])

dnl Keep these near the bottom - adding -Werror breaks various tests
AX_CFLAGS_WARN_ALL
AX_CXXFLAGS_WARN_ALL
AX_APPEND_COMPILE_FLAGS([-Werror])

AC_CONFIG_FILES(Makefile)
AC_OUTPUT
