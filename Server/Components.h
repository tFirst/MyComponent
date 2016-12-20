#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include <windows.h>
#include "Interfaces.h"

void println(const char* str);

HRESULT __stdcall GetClassObject(const CLSID& clsid, const IID& iid, void** ppv);

#endif // COMPONENTS_H_INCLUDED
