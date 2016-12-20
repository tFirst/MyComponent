#include <stdio.h>
#include "Components.h"
#include "InClass.h"

//*************************************************

CAEx::CAEx()
{
    println("CAEx::CAEx");
    counter = 0;

    IUnknown *iUnk;
    CoCreateInstance(CLSID_CA, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**) &iUnk);

    iUnk->QueryInterface(IID_IInput, (void**) &delegateInput);
    iUnk->QueryInterface(IID_IOutput, (void**) &delegateOutput);
    iUnk->QueryInterface(IID_IInterpolation, (void**) &delegateInter);
    iUnk->QueryInterface(IID_IDispatch, (void**) &delegateDisp);
    iUnk->Release();
}

CAEx::~CAEx()
{
    println("CAEx::~CAEx");

    delegateInput->Release();
    delegateOutput->Release();
    delegateInter->Release();
    delegateDisp->Release();
}

HRESULT __stdcall CAEx::QueryInterface(const IID& iid, void** ppv)
{
    println("CAEx::QueryInterface");

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
    else if (iid==IID_IOutputEx)
    {
        *ppv = (IOutputEx*)this;
    }
    else if (iid==IID_IInterpolationEx)
    {
        *ppv = (IInterpolationEx*)this;
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

ULONG __stdcall CAEx::AddRef()
{
    println("CAEx::AddRef");
    counter = counter + 1;
    return counter;
}

ULONG __stdcall CAEx::Release()
{
    println("CAEx::Release");
    counter = counter - 1;
    if (counter==0)
    {
        delete this;
    }
    return counter;
}

//
HRESULT __stdcall CAEx::GetResultTriple(double **values, unsigned int *length)
{
    printf("resultInterTri.units = %d", resultInterTri.units);
    printf("resultInterTri.units ^ 3 = %d", resultInterTri.units * resultInterTri.units * resultInterTri.units);
    *length = resultInterTri.units * resultInterTri.units * resultInterTri.units;
    double *avalues = new double[resultInterTri.units * resultInterTri.units * resultInterTri.units];

    for (int i = 0; i < resultInterTri.units; i++)
    {
        for (int j = 0; j < resultInterTri.units; j++)
        {
            for(int k = 0; k < resultInterTri.units; k++)
            {
                avalues[i * resultInterTri.units * resultInterTri.units + j * resultInterTri.units + k] = resultInterTri.result[i][j][k];
            }
        }
    }

    *values = avalues;
    return S_OK;
}

HRESULT __stdcall CAEx::GetResultTripleElement(double *value, unsigned int i, unsigned int j, unsigned int k)
{
    *value = this->resultInterTri.result[i][j][k];
    return S_OK;
}

unsigned int __stdcall CAEx::GetResultTripleOutputUnits()
{
    return this->resultInterTri.units;
}

HRESULT __stdcall CAEx::PerformInterpolationTriple()
{
    unsigned int units = this->values[6];

    printf("[Ex] units = %d\n", units);

    InterTri tri(this->values[0], this->values[1], this->values[2], this->values[3], this->values[4], this->values[5], this->values[6]);
    vector< vector< vector<double> > > result;

    result.resize(units);
    for(int i = 0; i < units; i ++)
    {
        result[i].resize(units);
        for(int j = 0; j < units; j++)
        {
            result[i][j].resize(units);
        }
    }
    result = tri.interpol(tri.getX(), tri.getY(), tri.getZ());

    this->resultInterTri.result = result;
    this->resultInterTri.units = units;

    return S_OK;
}
//

HRESULT __stdcall CAEx::PushInputFromArray(int *values, unsigned int length)
{
    println("CAEx:PushInputFromArray");
    this->length = length;
    this->values = new int[length];
    memcpy(this->values, values, length*sizeof(int));

    return delegateInput->PushInputFromArray(values, length);
}

HRESULT __stdcall CAEx::GetOutputLineFirst(double **values, unsigned int *length)
{
    return delegateOutput->GetOutputLineFirst(values, length);
}

HRESULT __stdcall CAEx::GetOutputLineNext(double **values, unsigned int *length)
{
    return delegateOutput->GetOutputLineNext(values, length);
}

unsigned int __stdcall CAEx::GetOutputUnits()
{
    return delegateOutput->GetOutputUnits();
}

HRESULT __stdcall CAEx::PerformInterpolationBCube()
{
    return delegateInter->PerformInterpolationBCube();
}

//IDispatch (Begin)
HRESULT __stdcall CAEx::GetTypeInfoCount(UINT* pctinfo)
{
    println("CAEx:GetTypeInfoCount");
    return S_OK;
}

HRESULT __stdcall CAEx::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
    println("CAEx:GetTypeInfo");
    return S_OK;
}

HRESULT __stdcall CAEx::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
                                      LCID lcid, DISPID* rgDispId)
{
    println("CAEx:GetIDsOfNames");
    if (cNames!=1)
    {
        return E_NOTIMPL;
    }

    if (wcscmp(rgszNames[0],L"PerformInterpolationTriple")==0)
    {
        rgDispId[0] = 7;
    }
    else if (wcscmp(rgszNames[0],L"GetResultTripleElement")==0)
    {
        rgDispId[0] = 8;
    }
    else if (wcscmp(rgszNames[0],L"GetResultTripleOutputUnits")==0)
    {
        rgDispId[0] = 9;
    }
    else
    {
        return delegateDisp->GetIDsOfNames(riid, rgszNames, cNames, lcid, rgDispId);
    }

    return S_OK;
}

HRESULT __stdcall CAEx::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,WORD wFlags, DISPPARAMS* pDispParams,VARIANT* pVarResult,
                               EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
    println("CAEx:Invoke");

    bool isBoolResult = false;
    HRESULT result;

    if (dispIdMember==1)
    {
        DISPPARAMS param = *pDispParams;
        VARIANT arg1 = (param.rgvarg)[0];

        if (V_ISARRAY(&arg1)) {
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
    else if (dispIdMember==7)
    {
        result = PerformInterpolationTriple();
        isBoolResult = true;
    }
    else if (dispIdMember==8) // GetResultTripleElement
    {
        if (pVarResult != NULL)
        {
            DISPPARAMS param = *pDispParams;
            VARIANT arg1 = (param.rgvarg)[0];
            VARIANT arg2 = (param.rgvarg)[1];
            VARIANT arg3 = (param.rgvarg)[2];

            VariantChangeType(&arg1, &arg1, 0, VT_UINT);
            VariantChangeType(&arg2, &arg2, 0, VT_UINT);
            VariantChangeType(&arg3, &arg3, 0, VT_UINT);

            double result;
            GetResultTripleElement(&result, arg1.uintVal, arg2.uintVal, arg3.uintVal);

            pVarResult->vt = VT_R8;
            pVarResult->dblVal = result;
        }
    }
    else if (dispIdMember==9)
    {
        if (pVarResult != NULL)
        {
            pVarResult->vt = VT_I2;
            pVarResult->iVal = GetResultTripleOutputUnits();
        }
    }
    else
    {
        return delegateDisp->Invoke(dispIdMember, riid, lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
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


CFAEx::CFAEx()
{
    println("CFAEx::CFAEx");
    counter = 0;
}

CFAEx::~CFAEx()
{
    println("CFAEx::~CFAEx");
}


HRESULT __stdcall CFAEx::QueryInterface(const IID& iid, void** ppv)
{
    println("CFAEx::QueryInterface");

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

ULONG __stdcall CFAEx::AddRef()
{
    println("CFAEx::AddRef");
    counter = counter + 1;
    return counter;
}

ULONG __stdcall CFAEx::Release()
{
    println("CFAEx::Release");
    counter = counter - 1;
    if (counter==0)
    {
        delete this;
    }
    return counter;
}


HRESULT __stdcall CFAEx::CreateInstance(IUnknown* pUnknownOuter, const IID& iid, void** ppv)
{
    println("CFAEx::CreateInstance");
    if (pUnknownOuter!=NULL)
    {
        return E_NOTIMPL;
    }

    CAEx* a = new CAEx();
    return a->QueryInterface(iid,ppv);
}

HRESULT __stdcall CFAEx::LockServer(BOOL bLock)
{
    println("CFAEx::LockServer");
    return S_OK;
}

//*************************************************


void println(const char* str)
{
    printf("[Ex] %s\n", str);
}

HRESULT __stdcall GetClassObject(const CLSID& clsid, const IID& iid, void** ppv)
{
    println("Component::GetClassObject");
    if (clsid==CLSID_CAEx)
    {
        CFAEx* fa  = new CFAEx();
        return fa->QueryInterface(iid,ppv);
    }
    else
    {
        *ppv = NULL;
        return E_NOTIMPL;
    }

}

//*************************************************
