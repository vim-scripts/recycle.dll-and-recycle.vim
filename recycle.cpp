/*
 * recycling tool for Win32 gVim
 * made by kAtremer <katremer@yandex.ru>
 *
 * use MS Visual C++ 6.0
 * build with cl /LD recycle.cpp shell32.lib kernel32.lib
 * call with libcallnr('recycle', 'Recycle', 'd:\path\file.ext')
 * returns: 1 on ok, 0 on error, -1 on user cancel
 *
 * best use together with recycle.vim
 */

#include <windows.h>

#pragma comment(linker, "/merge:.rdata=.text")
#pragma comment(linker, "/merge:.data=.text")
#pragma comment(linker, "/merge:.reloc=.text")
#pragma comment(linker, "/FILEALIGN:512 /IGNORE:4078")
#pragma comment(linker, "/SECTION:.text,EWRX")
#pragma comment(linker, "/NODEFAULTLIB")
#pragma comment(linker, "/ENTRY:DllMain")
#pragma comment(linker, "/SUBSYSTEM:windows")
#pragma comment(linker, "/STACK:65536,65536")

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason, LPVOID lpReserved) {
    return TRUE;
}

extern "C" __declspec(dllexport) LONG Recycle(char* param) {
	if (!param[0]) {
		return 0;
	} else {
		int succeeded=0;
		HLOCAL bufmem=LocalAlloc(LMEM_FIXED, lstrlen(param)+3);
		char* buf=(char*) bufmem;
		lstrcpy(buf, param);
		buf[lstrlen(param)]='~';
		buf[lstrlen(param)+1]=0;
		buf[lstrlen(param)+2]=0;
		BOOL copied=CopyFile(param, buf, TRUE);
		if (!copied) {
			succeeded=0;
		} else {
			SHFILEOPSTRUCT fileop;
			fileop.hwnd=0;
			fileop.wFunc=FO_DELETE;
			fileop.pFrom=buf;
			fileop.pTo=NULL;
			fileop.fFlags=FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
			if (SHFileOperation(&fileop)) succeeded=0;
			else if (fileop.fAnyOperationsAborted) succeeded=-1;
			else succeeded=1;
		}
		LocalFree(bufmem);
		return succeeded;
	}
}

/* this version just deletes the file
 * no problem with it but Vim says [New] when the file is written
 * which is not true
extern "C" __declspec(dllexport) LONG Recycle(char* param) {
	if (!param[0]) {
		return 0;
	} else {
		int success=0;
		HLOCAL bufmem=LocalAlloc(LMEM_FIXED, lstrlen(param)+2);
		char* buf=(char*) bufmem;
		lstrcpy(buf, param);
		buf[lstrlen(param)+1]=0;
		SHFILEOPSTRUCT fileop;
		fileop.hwnd=0;
		fileop.wFunc=FO_DELETE;
		fileop.pFrom=buf;
		fileop.pTo=NULL;
		fileop.fFlags=FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
		if (SHFileOperation(&fileop)) success=0;
		else if (fileop.fAnyOperationsAborted) success=-1;
		else success=1;
		LocalFree(bufmem);
		return success;
	}
}
*/
