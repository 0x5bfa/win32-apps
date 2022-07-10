#include <stdio.h>
#include <Windows.h>
#include <locale.h>

int main(void) {
    setlocale(LC_ALL, L"");
    DWORD dw = GetConsoleStringWidth(L"制限されたアプリケーションパッケージa");
    wprintf(L"%d", dw);
}

DWORD GetConsoleStringWidth(LPCWSTR lpString) {
    DWORD dwWidth = 0;
    for (int i = 0; lpString[i] != NULL; i++) {
        if (0x0020 <= lpString[i] && lpString[i] <= 0x007f) {
            dwWidth++;
        }
        else if (0x200000 <= lpString[i] && lpString[i] <= 0x2fa1f) {
            dwWidth += 2;
        }
        else if (0x3040 <= lpString[i] && lpString[i] <= 0x9fff) {
            dwWidth += 2;
        }
        else {
            dwWidth += 2;
        }
    }
    return dwWidth;
}