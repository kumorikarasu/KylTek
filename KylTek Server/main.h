#ifndef MAIN_H_
#define MAIN_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SERVERSIZE 32 //max simutanious connections

//Fun Typedefs
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef signed short s16;
typedef signed int s32;
typedef signed long s64;

//#if WIN32
	#include "winsock2.h" //sockets	
//#elif
	//include Linux libraries here
//#endif

#include <shlwapi.h>
#include <tchar.h>
#include <iostream>
#include <stdio.h>

using namespace std;

#endif // MAIN_H_