# ===========================================================================
#   http://www.gnu.org/software/autoconf-archive/ax_prog_perl_version.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_PROG_FAUST_VERSION([VERSION],[ACTION-IF-TRUE],[ACTION-IF-FALSE])
#
# DESCRIPTION
#
#   Makes sure that perl supports the version indicated. If true the shell
#   commands in ACTION-IF-TRUE are executed. If not the shell commands in
#   ACTION-IF-FALSE are run. Note if $FAUST is not set (for example by
#   running AC_CHECK_PROG or AC_PATH_PROG) the macro will fail.
#
#   Example:
#
#     AC_PATH_PROG([FAUST],[faust])
#     AX_PROG_FAUST_VERSION([0.9.46],[ ... ],[ ... ])
#
#   This will check to make sure that the faust you have supports at least
#   version 0.9.46.
#
#   NOTE: This macro uses the $FAUST variable to perform the check.
#   The $FAUST_VERSION variable will be valorized with the detected
#   version.
#
# LICENSE
#
#   Copyright (c) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
#   Copyright (c) 2009 Francesco Salvestrini <salvestrini@users.sourceforge.net>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 12

AC_DEFUN([AX_PROG_FAUST_VERSION],[
    AC_REQUIRE([AC_PROG_SED])
    AC_REQUIRE([AC_PROG_GREP])

    AS_IF([test -n "$FAUST"],[
        ax_faust_version="$1"

        AC_MSG_CHECKING([for faust version])
        changequote(<<,>>)
        faust_version=`$FAUST --version 2>&1 \
          | $SED -n -e '/FAUST.*Version/b inspect
b
: inspect
s/.* Version \([0-9]*\.[0-9]*\.[0-9]*\).*/\1/;p'`
        changequote([,])
        AC_MSG_RESULT($faust_version)

	AC_SUBST([FAUST_VERSION],[$faust_version])

        AX_COMPARE_VERSION([$ax_faust_version],[le],[$faust_version],[
	    :
            $2
        ],[
	    :
            $3
        ])
    ],[
        AC_MSG_WARN([could not find the faust compiler])
        $3
    ])
])
