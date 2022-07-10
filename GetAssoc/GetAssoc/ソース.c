#include <stdio.h>
#include <windows.h>
#include <shlwapi.h>
#include <locale.h>
#pragma comment(lib,"shlwapi.lib")
#pragma warning (disable: 4996)

typedef struct FILE_ASSOC {
    TCHAR wszCommand[MAX_PATH];
    TCHAR wszExecutable[MAX_PATH];
    TCHAR wszDocName[MAX_PATH];
    TCHAR wszAppName[MAX_PATH];
    TCHAR wszNoOpen[MAX_PATH];
    TCHAR wszShellNewValue[MAX_PATH];
    TCHAR wszDdeCommand[MAX_PATH];
    TCHAR wszDdeIfExec[MAX_PATH];
    TCHAR wszDdeApp[MAX_PATH];
    TCHAR wszDdeTopic[MAX_PATH];
    TCHAR wszInfoTip[MAX_PATH];
    TCHAR wszQuickTip[MAX_PATH];
    TCHAR wszTileInfo[MAX_PATH];
    TCHAR wszContentType[MAX_PATH];
    TCHAR wszDefaultIcon[MAX_PATH];
    TCHAR wszShellExtension[MAX_PATH];
    TCHAR wszDropTarget[MAX_PATH];
    TCHAR wszDelegateExecute[MAX_PATH];
    TCHAR wszSupportedUriProtocols[MAX_PATH];
    TCHAR wszProgId[MAX_PATH];
    TCHAR wszAppId[MAX_PATH];
    TCHAR wszAppPublisher[MAX_PATH];
    TCHAR wszAppIconReference[MAX_PATH];
    TCHAR wszMax[MAX_PATH];
}FILE_ASSOC, *LPFILE_ASSOC;





BOOL GetExecutable(LPCTSTR pszExtension, LPFILE_ASSOC pfa) {
    TCHAR wszFile[MAX_PATH * 2]; DWORD dwFileSize = MAX_PATH * 2; HRESULT hr;

    for (int i = 1; i <= 23; i++) {
        hr = AssocQueryStringW(ASSOCF_NOTRUNCATE, i, pszExtension, NULL, wszFile, &dwFileSize);
        switch (i) {
        case 1:
            wcscpy(pfa->wszCommand, wszFile);
        case 2:
            wcscpy(pfa->wszExecutable, wszFile);
        case 3:
            wcscpy(pfa->wszDocName, wszFile);
        case 4:
            wcscpy(pfa->wszAppName, wszFile);
        case 5:
            wcscpy(pfa->wszNoOpen, wszFile);
        case 6:
            wcscpy(pfa->wszShellNewValue, wszFile);
        case 7:
            wcscpy(pfa->wszDdeCommand, wszFile);
        case 8:
            wcscpy(pfa->wszDdeIfExec, wszFile);
        case 9:
            wcscpy(pfa->wszDdeApp, wszFile);
        case 10:
            wcscpy(pfa->wszDdeTopic, wszFile);
        case 11:
            wcscpy(pfa->wszInfoTip, wszFile);
        case 12: 
            wcscpy(pfa->wszQuickTip, wszFile);
        case 13:
            wcscpy(pfa->wszTileInfo, wszFile);
        case 14:
            wcscpy(pfa->wszContentType, wszFile);
        case 15:
            wcscpy(pfa->wszDefaultIcon, wszFile);
        case 16:
            wcscpy(pfa->wszShellExtension, wszFile);
        case 17:
            wcscpy(pfa->wszDropTarget, wszFile);
        case 18:
            wcscpy(pfa->wszDelegateExecute, wszFile);
        case 19:
            wcscpy(pfa->wszSupportedUriProtocols, wszFile);
        case 20:
            wcscpy(pfa->wszProgId, wszFile);
        case 21:
            wcscpy(pfa->wszAppId, wszFile);
        case 22:
            wcscpy(pfa->wszAppPublisher, wszFile);
        case 23:
            wcscpy(pfa->wszAppIconReference, wszFile);
        }
    }
    if (FAILED(hr) || dwFileSize == 0) return TRUE;
    else return FALSE;
}




int main(void) {
    _wsetlocale(LC_ALL, L"");

    FILE_ASSOC fa;
    BOOL bRes = GetExecutable(L".xlsx", &fa);
    wprintf(L"%s\n\n", fa.wszCommand);
}
