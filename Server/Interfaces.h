#ifndef INTERFACES_H_INCLUDED
#define INTERFACES_H_INCLUDED

#include <windows.h>

const CLSID CLSID_CA = {0xa70c55df,0xe5ed,0x4179,{0xb2,0x6b,0xdc,0x81,0x83,0xb0,0x05,0xdb}};

const IID IID_IInput = {0x00000001,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
const IID IID_IOutput = {0x00000002,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
const IID IID_IInterpolation = {0x00000003,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};

class IInput: public IUnknown
{
public:
    virtual HRESULT __stdcall PushInputFromArray(int *values, unsigned int length) = 0;
};

class IOutput: public IUnknown
{
public:
    virtual HRESULT __stdcall GetOutputLineFirst(double **values, unsigned int *length) = 0;
    virtual HRESULT __stdcall GetOutputLineNext(double **values, unsigned int *length) = 0;
    virtual unsigned int __stdcall GetOutputUnits() = 0;
};

class IInterpolation: public IUnknown
{
public:
    virtual HRESULT __stdcall PerformInterpolationBCube() = 0;
};

#endif // INTERFACES_H_INCLUDED
