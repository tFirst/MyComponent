#ifndef INTERFACES_H_INCLUDED
#define INTERFACES_H_INCLUDED

#include <vector>
#include <iostream>
#include <windows.h>
using namespace std;

//00000000-0000-0000-C000-000000000046
const IID IID_IUnknown = {0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};

//00000001-0000-0000-C000-000000000046
const IID IID_IClassFactory = {0x00000001,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};

//00020400-0000-0000-C000-000000000046
const IID IID_IDispatch = {0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}};

const IID IID_IInput = {0x00000001,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
const IID IID_IOutput = {0x00000002,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};
const IID IID_IInterpolation = {0x00000003,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}};

class IInput: public IUnknown
{
public:
    virtual HRESULT __stdcall PushInputFromArray(int *values, unsigned int length) = 0;
    virtual HRESULT __stdcall PushInputFromLocalFile(BSTR filename) = 0;
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
