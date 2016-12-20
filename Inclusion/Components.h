#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include <windows.h>
#include "InterfacesEx.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void println(const char* str);

HRESULT __stdcall GetClassObject(const CLSID& clsid, const IID& iid, void** ppv);

class CAEx: public IInput, IOutput, IInterpolation, IDispatch, IOutputEx, IInterpolationEx
{
    int counter;
    int *values;
    unsigned int length;

    struct {
        vector< vector< vector<double> > > result;
        int units;
    } resultInterTri;

    IInput *delegateInput;
    IOutput *delegateOutput;
    IInterpolation *delegateInter;
    IDispatch *delegateDisp;
public:
    CAEx();
    virtual ~CAEx();

    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    // IInput
    virtual HRESULT __stdcall PushInputFromArray(int *values, unsigned int length);
    //

    // IOutput
    virtual HRESULT __stdcall GetOutputLineFirst(double **values, unsigned int *length);
    virtual HRESULT __stdcall GetOutputLineNext(double **values, unsigned int *length);
    virtual unsigned int __stdcall GetOutputUnits();
    //

    // IInterpolation
    virtual HRESULT __stdcall PerformInterpolationBCube();
    //

    // IOutputEx
    virtual HRESULT __stdcall GetResultTriple(double **values, unsigned int *length);
    virtual HRESULT __stdcall GetResultTripleElement(double *value, unsigned int i, unsigned int j, unsigned int k);
    virtual unsigned int __stdcall GetResultTripleOutputUnits();
    //

    // IInterpolationEx
    virtual HRESULT __stdcall PerformInterpolationTriple();
    //

    // IDispatch
    virtual HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo);
    virtual HRESULT __stdcall GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
    virtual HRESULT __stdcall GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
    virtual HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams,VARIANT* pVarResult,
                                     EXCEPINFO* pExcepInfo, UINT* puArgErr);
    //
};

class CFAEx: public IClassFactory
{
private:
    int counter;
public:
    CFAEx();
    virtual ~CFAEx();

    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv);
    virtual HRESULT __stdcall LockServer(BOOL bLock);
};

#endif // COMPONENTS_H_INCLUDED
