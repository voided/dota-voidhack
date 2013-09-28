
#include "vh.h"

#include <windows.h>



BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		VH().Init();
	}
	else if ( fdwReason == DLL_PROCESS_DETACH )
	{
		VH().Shutdown();
	}

	return TRUE;
}
