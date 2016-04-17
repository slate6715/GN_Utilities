// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef WIN32
#include "stdheaders.h"
#else

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <Windows.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <sys/stat.h>


#include "miscutils.h"
// #include "GNUtilities.h"

#endif
