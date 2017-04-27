# Autoconf Requirements
# Initialization
AC_INIT([libGeoUtilities], [0.1])

# Safety check
AC_CONFIG_SRCDIR([src/miscutils.cpp])

# Where to store all the config details
AC_CONFIG_HEADERS([include/GeoUtilConfig.h])

# Put autotools auxillary files in a subdir to keep things clean
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Enable "automake" to simplify creating makefiles
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_FILES([Makefile])

# Look for a C compiler
AC_PROG_CC
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11
AC_PROG_LIBTOOL
# AC_PROG_RANLIB
AX_BOOST_BASE([1.48], [], AC_MSG_ERROR([blah is missing.]))
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM

# Do final output
AC_OUTPUT
