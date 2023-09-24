/*
File:   compile_me.c
Author: Taylor Robbins
Date:   09\23\2023
Description: 
	** A helper file that lets us easily build the library as a single compilation unit
*/

#include "std_format_defines.h"

#include "stdint.h"
#include "float.h"
#include "math.h"

#include "float.c"
#include "math.c"

#if COMPILER_GCC
#if STD_FORMAT_DLL
int DllMainCRTStartup() { return 0; }
#else
int mainCRTStartup() { return 0; }
#endif
#endif
