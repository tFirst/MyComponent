#include "main.h"
#include <stdio.h>
#include "Components.h"

HRESULT __stdcall DllGetClassObject(const CLSID& clsid, const IID& iid, void** ppv)
{
    println("Container::DllGetClassObject");
    return GetClassObject(clsid,iid,ppv);
}

//extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            g_hModule = (HMODULE) hinstDLL;
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}

BOOL HelperWriteKey(HKEY roothk, const char *lpSubKey, LPCSTR val_name, DWORD dwType, void *lpvData, DWORD dwDataSize) {
    //roothk:either of HKCR, HKLM, etc
    //lpSubKey: the key relative to 'roothk'
    //val_name:the key value name where the data will be written
    //dwType:the type of data that will be written ,REG_SZ,REG_BINARY, etc.
    //lpvData:a pointer to the data buffer
    //dwDataSize:the size of the data pointed to by lpvData

    HKEY hk;
    if (ERROR_SUCCESS != RegCreateKeyA(roothk, lpSubKey, &hk)) {
        return FALSE;
    }

    if (ERROR_SUCCESS != RegSetValueExA(hk, val_name, 0, dwType, (CONST BYTE *)lpvData, dwDataSize)) {
        return FALSE;
    }

    if (ERROR_SUCCESS != RegCloseKey(hk)) {
        return FALSE;
    }

    return TRUE;
}

HRESULT __stdcall DllRegisterServer() {
    WCHAR *lpwszClsid;

    char szBuff[MAX_PATH];
    char szClsid[MAX_PATH];
    char szInproc[MAX_PATH];
    char szProgId[MAX_PATH];
    char szDescriptionVal[256];

    StringFromCLSID(CLSID_CAEx, &lpwszClsid);

    sprintf(szClsid, "%S", lpwszClsid);
    sprintf(szInproc, "%s\\%s\\%s", "clsid", szClsid, "InprocServer32");
    sprintf(szProgId, "%s\\%s\\%s", "clsid", szClsid, "ProgId");

    sprintf(szBuff, "%s", "Interpolation Ex");
    sprintf(szDescriptionVal, "%s\\%s", "clsid", szClsid);

    if (HelperWriteKey(HKEY_CLASSES_ROOT, szDescriptionVal, NULL, REG_SZ, (void*)szBuff, strlen(szBuff)) == FALSE) {
        return 0;
    }

    GetModuleFileNameA(g_hModule, szBuff, sizeof(szBuff));
    if (HelperWriteKey(HKEY_CLASSES_ROOT, szInproc, NULL, REG_SZ, (void*)szBuff, strlen(szBuff)) == FALSE) {
        return 0;
    }

    strcpy(szBuff, ProgId);
    if (HelperWriteKey(HKEY_CLASSES_ROOT, szProgId, NULL, REG_SZ, (void*)szBuff, strlen(szBuff)) == FALSE) {
        return 0;
    }

    sprintf(szBuff, "%s", "Interpolation");
    if (HelperWriteKey(HKEY_CLASSES_ROOT, ProgId, NULL, REG_SZ, (void*)szBuff, strlen(szBuff)) == FALSE) {
        return 0;
    }

    sprintf(szProgId, "%s\\%s", ProgId, "CLSID");
    if (HelperWriteKey(HKEY_CLASSES_ROOT, szProgId, NULL, REG_SZ, (void*)szClsid, strlen(szClsid)) == FALSE) {
        return 0;
    }

    return 1;
}

HRESULT __stdcall DllUnregisterServer() {

    char szKeyName[256];
    char szClsid[256];
    WCHAR *lpwszClsid;

    sprintf(szKeyName, "%s\\%s", ProgId, "CLSID");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, szKeyName);
    RegDeleteKeyA(HKEY_CLASSES_ROOT, ProgId);

    StringFromCLSID(CLSID_CA, &lpwszClsid);
    sprintf(szClsid, "%S", lpwszClsid);
    sprintf(szKeyName, "%s\\%s\\%s", "CLSID", szClsid, "InprocServer32");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, szKeyName);

    sprintf(szKeyName, "%s\\%s\\%s", "CLSID", szClsid, "ProgId");
    RegDeleteKeyA(HKEY_CLASSES_ROOT, szKeyName);

    sprintf(szKeyName, "%s\\%s", "CLSID", szClsid);
    RegDeleteKeyA(HKEY_CLASSES_ROOT, szKeyName);

    return 1;
}

HRESULT __stdcall DllCanUnloadNow() {
    return g_ObjectsInUse == 0 ? S_OK : S_FALSE;
}
