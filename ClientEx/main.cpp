#include <objbase.h>
#include <iostream>
#include <stdio.h>
#include <locale>
#include "../Inclusion/InterfacesEx.h"
using namespace std;

int main()
{
    setlocale(LC_ALL, "");
    printf("Main::Start\n");


    int var = 0;
    //var = 1; //Using manager emulator
    //var = 2; //Using COM library (factory)
    var = 3; //Using COM library (instance)


    //Initializing COM library (Begin)
    CoInitialize(NULL);
    //Initializing COM library (End)

    try
    {
        IInput* pInput = NULL;

        HRESULT resInstance;

        //Getting instance (Begin)
        resInstance = CoCreateInstance(CLSID_CAEx,NULL,CLSCTX_INPROC_SERVER,IID_IInput,(void**)&pInput);

        if (!(SUCCEEDED(resInstance)))
        {

            //printf("%X\n",(unsigned int)resInstance);
            throw "No instance";
        }
        //Getting instance (End)


        //Work (Begin)

        int values[7] = {1, 2, 3, 4, 5, 6, 7};
        pInput->PushInputFromArray(values, 7);

        //Work (End)

        IInterpolationEx* pInterpolation = NULL;
        HRESULT resQuery = pInput->QueryInterface(IID_IInterpolationEx,(void**)&pInterpolation);
        if (!(SUCCEEDED(resQuery)))
        {
            //printf("%X\n",(unsigned int)resQuery);
            throw "No query";
        }

        pInterpolation->PerformInterpolationTriple();

        IOutputEx* pOutput = NULL;
        resQuery = pInput->QueryInterface(IID_IOutputEx,(void**)&pOutput);
        if (!(SUCCEEDED(resQuery)))
        {
            //printf("%X\n",(unsigned int)resQuery);
            throw "No query";
        }

        double* darr;
        unsigned int l;
        unsigned int units = pOutput->GetResultTripleOutputUnits();
        cout << units << endl << endl << endl;

        pOutput->GetResultTriple(&darr, &l);

        cout << "l = " << l << endl;

        for (int i = 0; i < units; i ++){
            for(int j = 0; j < units; j ++){
                    for(int k = 0; k < units; k++)
                        cout << darr[i * units * units + j * units + k] << "   ";
            cout << endl;
            }
            cout << endl << endl;
        }
        //Query (End)

        //Dispatch (Begin)
        IDispatch* pDisp = NULL;
        HRESULT resQueryDisp = pInput->QueryInterface(IID_IDispatch,(void**)&pDisp);
        if (!(SUCCEEDED(resQueryDisp)))
        {
            //printf("%X\n",(unsigned int)resQuery);
            throw "No query dispatch";
        }

        DISPID dispid;

        int namesCount = 1;
        OLECHAR** names = new OLECHAR*[namesCount];
        OLECHAR* name = const_cast<OLECHAR*>(L"GetResultTripleOutputUnits");
        names[0] = name;
        HRESULT resID_Name = pDisp->GetIDsOfNames(
                                 IID_NULL, // Должно быть IID_NULL
                                 names, // Имя функции
                                 namesCount, // Число имен
                                 GetUserDefaultLCID(), // Информация локализации
                                 &dispid
                             );
        if (!(SUCCEEDED(resID_Name)))
        {
            //printf("%X\n",(unsigned int)resID_Name);
            printf("No ID of name\n");
        }
        else
        {
            DISPPARAMS dispparamsNoArgs =
            {
                NULL,
                NULL,
                0, // Ноль аргументов
                0, // Ноль именованных аргументов
            };

            VARIANT V;
            HRESULT resInvoke = pDisp->Invoke(
                                    dispid, // DISPID
                                    IID_NULL, // Должно быть IID_NULL
                                    GetUserDefaultLCID(), // Информация локализации
                                    DISPATCH_METHOD, // Метод
                                    &dispparamsNoArgs, // Аргументы метода
                                    &V, // Результаты
                                    NULL, // Исключение
                                    NULL
                                ); // Ошибка в аргументе
            if (!(SUCCEEDED(resInvoke)))
            {
                printf("%X\n",(unsigned int)resInvoke);
                printf("Invoke error\n");
            }
            else
            {
                cout << V.iVal << endl;
            }
        }
        pDisp->Release();
        //Dispatch (End)

        //Deleting (Begin)
        pInterpolation->Release();
        pOutput->Release();

        //Query (Begin)

        pInput->Release();
        //Deleting (End)
    }
    catch (const char* str)
    {
        printf("Main::Error: ");
        printf("%s", str);
        printf("\n");
    }
    catch (...)
    {
        printf("Main::Error: Unknown\n");
    }


    //Uninitializing COM library (Begin)
    CoUninitialize();
    //Uninitializing COM library (End)

    printf("Main::Finish\n");
    return 0;
}
