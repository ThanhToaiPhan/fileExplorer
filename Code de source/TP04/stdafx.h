// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib,"comctl32.lib ")
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <ObjIdl.h>
#include <gdiplus.h>
#include <CommCtrl.h>
#include <WindowsX.h>
#include <tchar.h>
#include <strsafe.h>
#include <Windows.h>
#include <string>
#include <ShellAPI.h>
#include <Shlwapi.h>
#include <thumbcache.h>
#include <propsys.h>