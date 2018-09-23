
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
#include <SDKDDKVer.h>
#include <windows.h>
#define WIN32_LEAN_AND_MEAN

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif