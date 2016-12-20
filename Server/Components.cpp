#include <stdio.h>
#include "Components.h"
#include "InClass.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

//*************************************************

class CA: public IInput, IOutput, IInterpolation, IDispatch
{
private:
    struct
    {
        vector< vector<double> > result;
        int units;
        vector< vector<double> >::iterator resultIterator;
    } resultInterBCube;
    int counter;
    int px1;
    int *values;
    int units;
    unsigned int length;

    double *outValues;
    unsigned int outLength;
public:
    CA();
    virtual ~CA();

    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    virtual HRESULT __stdcall PushInputFromArray(int *values, unsigned int length);

    virtual HRESULT __stdcall GetOutputLineFirst(double **values, unsigned int *length);
    virtual HRESULT __stdcall GetOutputLineNext(double **values, unsigned int *length);
    virtual unsigned int __stdcall GetOutputUnits();

    virtual HRESULT __stdcall PerformInterpolationBCube();

    virtual HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo);
    virtual HRESULT __stdcall GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
    virtual HRESULT __stdcall GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
    virtual HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams,VARIANT* pVarResult,
                                     EXCEPINFO* pExcepInfo, UINT* puArgErr);

};

CA::CA()
{
    println("CA::CA");
    counter = 0;
    px1=123;
}

CA::~CA()
{
    println("CA::~CA");
}


HRESULT __stdcall CA::QueryInterface(const IID& iid, void** ppv)
{
    println("CA::QueryInterface");

    if (iid==IID_IUnknown)
    {
        *ppv = (IUnknown*)(IInput*)this;
    }
    else if (iid==IID_IInput)
    {
        *ppv = (IInput*)this;
    }
    else if (iid==IID_IOutput)
    {
        *ppv = (IOutput*)this;
    }
    else if (iid==IID_IInterpolation)
    {
        *ppv = (IInterpolation*)this;
    }
    else if (iid==IID_IDispatch)
    {
        *ppv = (IDispatch*)this;
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    this->AddRef();
    return S_OK;
}

ULONG __stdcall CA::AddRef()
{
    println("CA::AddRef");
    counter = counter + 1;
    return counter;
}

ULONG __stdcall CA::Release()
{
    println("CA::Release");
    counter = counter - 1;
    if (counter==0)
    {
        delete this;
    }
    return counter;
}

HRESULT __stdcall CA::PushInputFromArray(int *values, unsigned int length)
{
    println("CA:PushInputFromArray");
    this->length = length;
    this->values = new int[length];
    memcpy(this->values, values, length*sizeof(int));

    return S_OK;
}

HRESULT __stdcall CA::GetOutputLineFirst(double **values, unsigned int *length)
{
    resultInterBCube.resultIterator = resultInterBCube.result.begin();
    if (resultInterBCube.resultIterator != resultInterBCube.result.end())
    {
        vector<double> line = *resultInterBCube.resultIterator;
        *values = new double[line.size()];
        *length = line.size();
        std::copy(line.begin(), line.end(), *values);
        ++resultInterBCube.resultIterator;
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

HRESULT __stdcall CA::GetOutputLineNext(double **values, unsigned int *length)
{
    if (resultInterBCube.resultIterator != resultInterBCube.result.end())
    {
        vector<double> line = *resultInterBCube.resultIterator;
        *values = new double[line.size()];
        *length = line.size();
        std::copy(line.begin(), line.end(), *values);
        ++resultInterBCube.resultIterator;
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

unsigned int __stdcall CA::GetOutputUnits()
{
    return resultInterBCube.units;
}

HRESULT __stdcall CA::PerformInterpolationBCube()
{
    println("CA:InterpolationBCube");
    int units = values[4];
    InterBi inclass(values[0], values[1], values[2], values[3], units);
    vector< vector<double> > result;
    result.resize(units);
    for(int i = 0; i < units; i ++)
    {
        result[i].resize(units);
    }
    result = inclass.interpol(inclass.getX(), inclass.getY());
    resultInterBCube.result = result;
    resultInterBCube.units = units;

    return S_OK;
}

//IDispatch (Begin)
HRESULT __stdcall CA::GetTypeInfoCount(UINT* pctinfo)
{
    println("CA:GetTypeInfoCount");
    return S_OK;
}

HRESULT __stdcall CA::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
    println("CA:GetTypeInfo");
    return S_OK;
}

HRESULT __stdcall CA::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
                                    LCID lcid, DISPID* rgDispId)
{
    println("CA:GetIDsOfNames");
    if (cNames!=1)
    {
        return E_NOTIMPL;
    }

    if (wcscmp(rgszNames[0],L"PushInputFromArray")==0)
    {
        rgDispId[0] = 1;
    }
    else if (wcscmp(rgszNames[0],L"GetOutputLineFirst")==0)
    {
        rgDispId[0] = 2;
    }
    else if (wcscmp(rgszNames[0],L"GetOutputLineNext")==0)
    {
        rgDispId[0] = 3;
    }
    else if (wcscmp(rgszNames[0],L"GetOutputUnits")==0)
    {
        rgDispId[0] = 4;
    }
    else if (wcscmp(rgszNames[0],L"PerformInterpolationBCube")==0)
    {
        rgDispId[0] = 5;
    }
    else if (wcscmp(rgszNames[0],L"OutputLine")==0)
    {
        rgDispId[0] = 6;
    }
    else
    {
        return E_NOTIMPL;
    }
    return S_OK;
}

HRESULT __stdcall CA::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams,VARIANT* pVarResult,
                             EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
    println("CA:Invoke");

    bool isBoolResult = false;
    HRESULT result;

    if (dispIdMember==1)
    {
        DISPPARAMS param = *pDispParams;
        VARIANT arg1 = (param.rgvarg)[0];

        if (V_ISARRAY(&arg1))
        {
            VARIANT *pVals;
            SAFEARRAY *sa = arg1.parray;
            SafeArrayAccessData(sa, (void**) &pVals);

            long iLBound;
            long iUBound;

            SafeArrayGetLBound(sa, 1, &iLBound);
            SafeArrayGetUBound(sa, 1, &iUBound);

            int index = 0;
            int _count = iUBound - iLBound + 1;
            int *arr = new int[_count];

            for (long int i = 0; i < iUBound - iLBound + 1; i++)
            {
                VARIANT arg = pVals[i];
                VariantChangeType(&arg, &arg, 0, VT_UINT);

                arr[index++] = arg.uintVal;
            }

            SafeArrayUnaccessData(sa);
            PushInputFromArray(arr, _count);
        }
    }
    else if (dispIdMember==2)
    {
        result = GetOutputLineFirst(&outValues, &outLength);
        isBoolResult = true;
    }
    else if (dispIdMember==3)
    {
        result = GetOutputLineNext(&outValues, &outLength);
        isBoolResult = true;
    }
    else if (dispIdMember==4)
    {
        if (pVarResult != NULL)
        {
            pVarResult->vt = VT_I2;
            pVarResult->iVal = GetOutputUnits();
        }
    }
    else if (dispIdMember==5)
    {
        PerformInterpolationBCube();
    }
    else if (dispIdMember==6)
    {
        if (pVarResult != NULL)
        {
            pVarResult->vt = VT_VARIANT | VT_ARRAY;
            SAFEARRAY *sa;
            SAFEARRAYBOUND sabound[1];

            // Create an array of size outLength
            sabound[0].cElements = outLength;
            sabound[0].lLbound = 0;

            sa = SafeArrayCreate(VT_VARIANT, 1, sabound);

            VARIANT *pVals;
            SafeArrayAccessData(sa, (void**)&pVals);

            for (unsigned int i = 0; i < outLength; i++)
            {
                VARIANT arg;
                VariantInit(&arg);

                arg.vt = VT_R8;
                arg.dblVal = outValues[i];

                pVals[i] = arg;
            }

            SafeArrayUnaccessData(sa);
            V_ARRAY(pVarResult) = sa;
        }
    }
    else
    {
        return E_NOTIMPL;
    }

    if (isBoolResult && pVarResult != NULL)
    {
        pVarResult->vt = VT_BOOL;
        pVarResult->boolVal = result == S_OK ? VARIANT_TRUE : VARIANT_FALSE;
    }

    return S_OK;
}
//IDispatch (End)


//*************************************************


class CFA: public IClassFactory
{
private:
    int counter;
public:
    CFA();
    virtual ~CFA();

    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
    virtual ULONG __stdcall AddRef();
    virtual ULONG __stdcall Release();

    virtual HRESULT __stdcall CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv);
    virtual HRESULT __stdcall LockServer(BOOL bLock);
};

CFA::CFA()
{
    println("CFA::CFA");
    counter = 0;
}

CFA::~CFA()
{
    println("CFA::~CFA");
}


HRESULT __stdcall CFA::QueryInterface(const IID& iid, void** ppv)
{
    println("CFA::QueryInterface");

    if (iid==IID_IUnknown)
    {
        *ppv = (IUnknown*)(IClassFactory*)this;
    }
    else if (iid==IID_IClassFactory)
    {
        *ppv = (IClassFactory*)this;
    }
    else
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }
    this->AddRef();
    return S_OK;
}

ULONG __stdcall CFA::AddRef()
{
    println("CFA::AddRef");
    counter = counter + 1;
    return counter;
}

ULONG __stdcall CFA::Release()
{
    println("CFA::Release");
    counter = counter - 1;
    if (counter==0)
    {
        delete this;
    }
    return counter;
}


HRESULT __stdcall CFA::CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv)
{
    println("CFA::CreateInstance");
    if (pUnknownOuter!=NULL)
    {
        return E_NOTIMPL;
    }

    CA* a = new CA();
    return a->QueryInterface(iid,ppv);
}

HRESULT __stdcall CFA::LockServer(BOOL bLock)
{
    println("CFA::LockServer");
    return S_OK;
}

//*************************************************


void println(const char* str)
{
    printf(str);
    printf("\n");
}

HRESULT __stdcall GetClassObject(const CLSID& clsid, const IID& iid, void** ppv)
{
    println("Component::GetClassObject");
    if (clsid==CLSID_CA)
    {
        CFA* fa  = new CFA();
        return fa->QueryInterface(iid,ppv);
    }
    else
    {
        *ppv = NULL;
        return E_NOTIMPL;
    }

}

//*************************************************
