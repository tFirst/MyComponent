#ifndef INTERFACESEX_H_INCLUDED
#define INTERFACESEX_H_INCLUDED

#include <windows.h>
#include "../Server/Interfaces.h"


//09547c04-5b67-4c41-ad93-6bfdfce8525e
const CLSID CLSID_CAEx = {0x09547c04,0x5b67,0x4c41,{0xad,0x93,0x6b,0xfd,0xfc,0xe8,0x52,0x5e}};

const IID IID_IOutputEx = {0x00000002,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02}};
const IID IID_IInterpolationEx = {0x00000003,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03}};

class IOutputEx: public IUnknown
{
public:
    virtual HRESULT __stdcall GetResultTriple(double **values, unsigned int *length) = 0;
    virtual HRESULT __stdcall GetResultTripleElement(double *value, unsigned int i, unsigned int j, unsigned int k) = 0;
    virtual unsigned int __stdcall GetResultTripleOutputUnits() = 0;
};

class IInterpolationEx: public IUnknown
{
public:
    virtual HRESULT __stdcall PerformInterpolationTriple() = 0;
};

#endif // INTERFACESEX_H_INCLUDED
