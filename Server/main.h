#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif

static HMODULE g_hModule = NULL;
static ULONG g_ObjectsInUse = 0;

#define ProgId "Trushin.Interpolation"

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT HRESULT __stdcall DllGetClassObject(const CLSID& clsid, const IID& iid, void** ppv);

DLL_EXPORT HRESULT __stdcall DllRegisterServer();
DLL_EXPORT HRESULT __stdcall DllUnregisterServer();
DLL_EXPORT HRESULT __stdcall DllCanUnloadNow();

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
