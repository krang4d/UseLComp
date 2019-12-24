#include <iostream>
#include <stdio.h>
#include <dlfcn.h>
#include "stubs.h"
//#include "ifc_ldev.h"
//#include "e154.h"
#define INITGUID

#include "stubs.h"
#include "ioctl.h"
#include "e2010cmd.h"
#include "791cmd.h"
#include "ifc_ldev.h"
#include <errno.h>

using namespace std;

typedef IDaqLDevice* (*CREATEFUNCPTR)(ULONG Slot);
CREATEFUNCPTR CreateInstance;

void *handle;
HANDLE hVxd;
SLOT_PAR sl;
PLATA_DESCR_U2 pd;
char *error;

int main()
{
    handle = dlopen("./liblcomp.so", RTLD_LAZY);
    if(!handle)
    {
        fprintf(stderr, "dlopen() error: %s\n", dlerror());
        return 1;
    }

    CreateInstance =(CREATEFUNCPTR) dlsym(handle,"CreateInstance");
    if((error = dlerror())!=NULL)
    {
       cout << error << endl;
    }

    char *name = "e154";
    LUnknown* pIUnknown = CreateInstance(0);
//    for(ULONG i = 0; i<127; i++)
//    {
//        pIUnknown = CreateInstance(i);
//        cout << errno << endl;
//        if(pIUnknown != NULL) break;
//    }

    if(pIUnknown == NULL)
    {
        cout << "CallCreateInstance failed " << endl;
        return 1;
    }
    cout << "Get IDaqLDevice interface" << endl;
    IDaqLDevice* pI;
    HRESULT hr = pIUnknown->QueryInterface(IID_ILDEV,(void**)&pI);
    if(hr!=S_OK)
    {
        cout << "Get IDaqLDevice failed" << endl;
        return 1;
    }
    printf("IDaqLDevice get success \n");
    pIUnknown->Release();
    cout << "Free IUnknown" << endl;

    cout << "OpenLDevice Handle" << hex << (hVxd=pI->OpenLDevice()) << endl;

    cout << endl << "Slot parameters" << endl;
    pI->GetSlotParam(&sl);
    cout << endl << "Slot parameters" << endl;
    pI->GetSlotParam(&sl);

    cout << "Base    " << hex << sl.Base << endl;
    cout << "BaseL   " << sl.BaseL << endl;
    cout << "Mem     " << sl.Mem << endl;
    cout << "MemL    " << sl.MemL << endl;
    cout << "Type    " << sl.BoardType << endl;
    cout << "DSPType " << sl.DSPType << endl;
    cout << "Irq     " << sl.Irq << endl;

    cout << "Load Bios " << pI->LoadBios("e154") << endl;
    cout << "Plata Test " << pI->PlataTest() << endl;

    cout << endl << "Read FLASH" << endl;

    pI->ReadPlataDescr(&pd); // fill up properties

    cout << "SerNum       " << pd.t7.SerNum << endl;
    cout << "BrdName      " << pd.t7.BrdName << endl;
    cout << "Rev          " << pd.t7.Rev << endl;
    cout << "DspType      " << pd.t7.DspType << endl;
    cout << "IsDacPresent " << pd.t7.IsDacPresent << endl;
    cout << "Quartz       " << dec << pd.t7.Quartz << endl;
    pI->StopLDevice();
    pI->CloseLDevice();
    return 0;
}

