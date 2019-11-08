# Autoconf Requirements
# Initialization
AC_PREREQ([2.69])
AC_INIT([libGeoUtilities], [0.1])
LT_INIT
AC_CONFIG_SRCDIR([src/miscutils.cpp])
AC_CONFIG_HEADERS([include/GeoUtilConfig.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
# AX_CXX_COMPILE_STDCXX_11
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL


# Checks for libraries.
# AX_BOOST_BASE([1.48],AC_DEFINE([USE_BOOST], [], [Define if the boost library should be used.]),
#               AX_BOOST_SYSTEM
#               AX_BOOST_FILESYSTEM
#               , 
#               AC_MSG_NOTICE(["Boost is not installed. Some library features will not be available"]) )


# Checks for header files.
AC_CHECK_HEADERS([limits.h netinet/in.h stddef.h stdlib.h string.h sys/socket.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([bzero memmove memset pow regcomp socket sqrt strchr strtoul])

# Put autotools auxillary files in a subdir to keep things clean
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Enable "automake" to simplify creating makefiles
AM_INIT_AUTOMAKE([subdir-objects -Wall ])
AC_CONFIG_FILES([Makefile
		 src/Makefile])

# Do final output
AC_OUTPUT
