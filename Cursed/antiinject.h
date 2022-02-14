#include <atlstr.h>

BOOLEAN BlockAPI(HANDLE, CHAR*, CHAR*);
void AntiInject();

void AntiInject() {
    HANDLE hProc = GetCurrentProcess();
    while (TRUE) {
        BlockAPI(hProc, unconstchar("NTDLL.DLL"), unconstchar("LdrLoadDll"));
        Sleep(100);
    }
}

BOOLEAN BlockAPI(HANDLE hProcess, CHAR* libName, CHAR* apiName) {
    CHAR pRet[] = { 0x31, 0xC0, 0xC3 }; 
    HINSTANCE hLib = NULL;
    VOID* pAddr = NULL;
    BOOL bRet = FALSE;
    SIZE_T dwRet = 0;

    hLib = LoadLibrary(CA2CT(libName));
    if (hLib) {
        pAddr = (VOID*)GetProcAddress(hLib, apiName);
        if (pAddr) {
            if (WriteProcessMemory(hProcess, (LPVOID)pAddr, (LPCVOID)pRet, sizeof(pRet), &dwRet)) {
                if (dwRet) {
                    bRet = TRUE;
                }
            }
        }
        FreeLibrary(hLib);
    }
    return bRet;
}