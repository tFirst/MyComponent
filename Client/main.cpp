#include <objbase.h>
#include <iostream>
#include <stdio.h>
#include <locale>
#include "../Server/Interfaces.h"
using namespace std;

//Manager emulator method
typedef HRESULT __stdcall (*GetClassObjectType) (const CLSID& clsid, const IID& iid, void** ppv);

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

    try {
     IInput* pInput = NULL;

     HRESULT resInstance;

     //Getting instance (Begin)
     resInstance = CoCreateInstance(CLSID_CA,NULL,CLSCTX_INPROC_SERVER,IID_IInput,(void**)&pInput);

     if (!(SUCCEEDED(resInstance)))
     {
          //printf("%X\n",(unsigned int)resInstance);
          throw "No instance";
     }
     //Getting instance (End)


     //Work (Begin)

         int values[5] = {1, 2, 3, 4, 5};
         pInput->PushInputFromArray(values, 5);

     //Work (End)

     IInterpolation* pInterpolation = NULL;
     HRESULT resQuery = pInput->QueryInterface(IID_IInterpolation,(void**)&pInterpolation);
     if (!(SUCCEEDED(resQuery)))
     {
          //printf("%X\n",(unsigned int)resQuery);
          throw "No query";
     }

     pInterpolation->PerformInterpolationBCube();

     IOutput* pOutput = NULL;
     resQuery = pInput->QueryInterface(IID_IOutput,(void**)&pOutput);
     if (!(SUCCEEDED(resQuery)))
     {
          //printf("%X\n",(unsigned int)resQuery);
          throw "No query";
     }

     double* d;
     unsigned int l;
     cout << pOutput->GetOutputUnits() << endl;
     if (pOutput->GetOutputLineFirst(&d, &l) == S_OK) {
            for (int i = 0; i < l; i++) {
                cout << d[i] << " ";
            }
            cout << endl;
        while(pOutput->GetOutputLineNext(&d, &l) == S_OK) {
            for (int i = 0; i < l; i++) {
                cout << d[i] << " ";
            }
            cout << endl;
        }
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
     OLECHAR* name = const_cast<OLECHAR*>(L"GetOutputUnits");
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
         DISPPARAMS dispparamsNoArgs = {
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
        }else{
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
        printf(str);
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
