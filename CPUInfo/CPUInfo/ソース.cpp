#include <stdio.h>
#include <Windows.h>

typedef struct CPU_VERSION {
    UINT stepping;
    UINT model;
    UINT family;
    UINT type;
    UINT exmodel;
    UINT exfamily;
}CPU_VERSION, *LPCPU_VERSION;



int wmain(void) {
    CPU_VERSION cver;
    GetCpuVersion(&cver);
    wprintf(L"Extension family: %x\n"
            L"Extension Model:  %x\n"
            L"Family:           %x\n"
            L"Model:            %x\n"
            L"Stepping:         %x\n"
            L"Type:             %x\n", cver.exfamily, cver.exmodel, cver.family, cver.model, cver.stepping, cver.type);
}

int GetCpuVersion(LPCPU_VERSION cver) {
    cver->exfamily = 0;
    cver->exmodel  = 4;
    cver->family   = 6;
    cver->model    = 5;
    cver->stepping = 2;
    cver->type     = 4;

    __asm __volatile__ ("movl $1, %%eax\n" "cpuid" : "=a"(cpuidNum) : 
    );



}