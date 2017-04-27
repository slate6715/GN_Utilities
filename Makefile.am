ACLOCAL_AMFLAGS = -I m4 --install

EXTRA_DIST = m4/NOTES bootstrap

lib_LTLIBRARIES = libGeoUtilities.la
libGeoUtilities_la_SOURCES = src/BinFile.cpp src/BufferedInsert.cpp src/BufferedReader.cpp src/CmdTCPConnection.cpp src/CommandHandler.cpp src/ConfigFile.cpp src/DBase.cpp src/Dictionary.cpp src/DurationTracker.cpp src/EMat.cpp src/Image.cpp src/KeywordMgr.cpp src/LinuxUDPServer.cpp src/miscutils.cpp src/MySQLDBase.cpp src/opencv_utils.cpp src/OutStream.cpp src/SSHClient.cpp src/SSHServer.cpp src/StdOutStream.cpp src/TarFile.cpp src/TarReader.cpp src/TCPConnection.cpp src/TCPServer.cpp src/tinystr.cpp src/tinyxml.cpp src/tinyxmlerror.cpp src/tinyxmlparser.cpp src/UDPClient.cpp src/UDPServer.cpp src/WinUDPClient.cpp src/WinUDPServer.cpp src/WordNet.cpp
libGeoUtilities_la_CPPFLAGS = $(BOOST_CPPFLAGS)
libGeoUtilities_la_LDFLAGS = $(BOOST_THREAD_LDFLAGS)
libGeoUtilities_la_LIBS = $(BOOST_THREAD_LIBS)

