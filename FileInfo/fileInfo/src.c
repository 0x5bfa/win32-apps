#include "fileInfo.h"
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

typedef struct _FILE_INFO {
    WCHAR wszFileName[MAX_PATH];
    WCHAR wszFullPathName[MAX_PATH];
    WCHAR wszFileExtension[MAX_PATH];
    WCHAR wszOpenWithAppName[MAX_PATH];
    WCHAR wszFileTypeName[MAX_PATH];
    DWORD dwBinaryType;
}FILE_INFO, *LPFILE_INFO;

typedef struct FILE_ASSOC {
    TCHAR wszCommand[MAX_PATH];         //ファイルを開くアプリの実行コマンド ()
    TCHAR wszExecutable[MAX_PATH];      //実行ファイルパス ()
    TCHAR wszDocName[MAX_PATH];         //ファイルの種類 (Microsoft Word 文書)
    TCHAR wszAppName[MAX_PATH];         //アプリケーションの名前 (Word)
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
    TCHAR wszDefaultIcon[MAX_PATH];     //ファイルアイコンのパス
    TCHAR wszShellExtension[MAX_PATH];
    TCHAR wszDropTarget[MAX_PATH];
    TCHAR wszDelegateExecute[MAX_PATH];
    TCHAR wszSupportedUriProtocols[MAX_PATH];
    TCHAR wszProgId[MAX_PATH];          //Programment ID
    TCHAR wszAppId[MAX_PATH];           //App ID
    TCHAR wszAppPublisher[MAX_PATH];    //実行アプリケーションの発行元
    TCHAR wszAppIconReference[MAX_PATH];
    TCHAR wszMax[MAX_PATH];
}FILE_ASSOC, * LPFILE_ASSOC;




int main(void) {
    setlocale(LC_ALL, "");
    WCHAR wszFileName[MAX_PATH] = L"C:\\Users\\T31068068\\Desktop\\word doc.docx";
    wprintf(L"===================================================================================================================\n");
    wprintf(L"file information of \"%s\"\n\n", wszFileName);

    FILE_INFO fi;
    GetFileInfo(wszFileName, &fi);
    return 0;
}


DWORD GetFileInfo(LPWSTR lpFileName, LPFILE_INFO pfi) {
    //完全パス・ファイル名の取得
    WCHAR wszFullPathName[MAX_PATH];
    WCHAR* wszFileName;
    GetFullPathNameW(lpFileName, MAX_PATH, wszFullPathName, &wszFileName);
    wcscpy(pfi->wszFullPathName, wszFullPathName);
    wcscpy(pfi->wszFileName, wszFileName);

    wcscpy(pfi->wszFileExtension, PathFindExtensionW(pfi->wszFullPathName));

    //実行ファイルかどうか・実行ファイルの種類
    GetBinaryTypeW(wszFileName, &pfi->dwBinaryType);

    //拡張子情報
    LPFILE_ASSOC fa = (LPFILE_ASSOC)malloc(sizeof(FILE_ASSOC));
    GetAssoc(pfi->wszFileExtension, fa);
    wprintf(L"%s\n\n", fa->wszDocName);


    //ファイルの一般情報取得
}


BOOL GetAssoc(LPCTSTR pszExtension, LPFILE_ASSOC pfa) {
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
